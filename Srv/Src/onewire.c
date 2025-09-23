/**
  ******************************************************************************
  * @file           : onewire.c
  *                   This file contains OneWire bus and dedicated GPIO 
  *                   initialization.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
 

/* Includes ------------------------------------------------------------------*/
#include "onewire.h"

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/




/*******************************************************************************/


/* Private macro -------------------------------------------------------------*/
#define OW_Low  PIN_H(OW_PORT, OW_PIN)
#define OW_High PIN_L(OW_PORT, OW_PIN)
#define OW_Level (PIN_LEVEL(OW_PORT, OW_PIN))

// #define delay_us vTaskDelay

/* Global variables ----------------------------------------------------------*/
uint32_t _OWREG_ = 0;
static SemaphoreHandle_t gOwMutex;

/* Private variables ---------------------------------------------------------*/
static uint8_t lastfork;
static ow_device_t ow_devices[2];


static void dwt_init(void) {
  DWT->CTRL, 0;
  DWT->CYCCNT = 0;
  SET_BIT(DWT->CTRL, DWT_CTRL_CYCEVTENA_Msk);
  // __DSB();
  // __ISB();
}

static void delay_us(uint32_t us) {
  // __asm volatile("nop");
  dwt_init();
  uint32_t const start = READ_REG(DWT->CYCCNT);
  uint32_t const ticks = us * (configCPU_CLOCK_HZ / 1000000u);
  while ((READ_REG(DWT->CYCCNT) - start) < ticks) { __asm volatile("nop"); }
  CLEAR_BIT(DWT->CTRL, DWT_CTRL_CYCEVTENA_Msk);
    
}


static BaseType_t ow_lock(TickType_t to)   { return xSemaphoreTake(gOwMutex, to); }
static void       ow_unlock(void)          { xSemaphoreGive(gOwMutex); }






// -------------------------------------------------------------  
void OW_Reset(void) {
  // gOwMutex = xSemaphoreCreateMutex();
  // dwt_init();
  FLAG_CLR(_OWREG_, _OLF_);

  taskENTER_CRITICAL();
  OW_High;
  delay_us(580);
  OW_Low;
  // delay_us(15);
  
  int i = 0;
  while (i++ < 240) {
    if (!OW_Level) {
      FLAG_SET(_OWREG_, _OLF_);
      break;
    }
    delay_us(1);
  }
  taskEXIT_CRITICAL();
  delay_us(580 - i);

}


// -------------------------------------------------------------  
void OW_WriteBit(uint8_t bit) {
  OW_High;
  if (bit) {
    delay_us(6);
    OW_Low;
    delay_us(64);
  } else {
    delay_us(60);
    OW_Low;
    delay_us(10);
  }
}


// -------------------------------------------------------------  
void OW_Write(uint8_t* data) {
  uint8_t _byte_ = *data;
  for (int i = 0; i < 8; i++) {
    OW_WriteBit(_byte_ & 0x01);
    _byte_ >>= 1;
  }
}


// -------------------------------------------------------------  
uint8_t OW_ReadBit(void) {
  OW_High;
  delay_us(6);
  OW_Low;
  delay_us(9);
  uint8_t level = OW_Level;
  delay_us(55);
  
  return level;
}


// -------------------------------------------------------------  
void OW_Read(uint8_t* data) {
  for (int i = 0; i < 8; i++) {
    *data >>= 1;
    *data |= (OW_ReadBit()) ? 0x80 : 0;
  }
}


// -------------------------------------------------------------  
void OW_CRC8(uint8_t* __crc, uint8_t __byte) {
  // 0x8c - it is a bit reverse of OneWire polinom of 0x31
  for (uint8_t i = 0; i < 8; i++) {
		*__crc = ((*__crc ^ (__byte >> i)) & 0x01) ? ((*__crc >> 1) ^ 0x8c) : (*__crc >> 1);
	}
}


// -------------------------------------------------------------  
int8_t OW_Error_Handler(void) {
  return (-1);
}










// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  







uint8_t OW_Enumerate(uint8_t* addr) {
	if (!lastfork) return (0);
  
	OW_Reset();
  // delay_us(100000);  

  if (!FLAG_CHECK(_OWREG_, _OLF_)) return (0);
  
  //addr += 7;
  uint8_t bp = 7;
	uint8_t prev = *addr;
	uint8_t curr = 0;
	uint8_t fork = 0;	
	uint8_t bit0 = 0;
	uint8_t bit1 = 0;
  
  uint8_t cmd = SearchROM;
	OW_Write(&cmd);

	for(uint8_t i = 1; i < 65; i++) {
    bit0 = OW_ReadBit();
    bit1 = OW_ReadBit();

		if (!bit0) { // ���� ������������ � ������� ��� ����
			if (!bit1) { // �� ����� ������������� ��� 1 (�����)
				if (i < lastfork) { // ���� �� ����� �������� ������� ������������ ����,
					if (prev & 1) {
						curr |= 0x80; // �� �������� �������� ���� �� �������� �������
					} else {
						fork = i; // ���� ����, �� �������� ����������� �����
					}
				} else if (i == lastfork) {
            curr |= 0x80; // ���� �� ���� ����� � ������� ��� ��� ������ �������� � ����, ������� 1
					} else {
            fork = i; // ������ - ������� ���� � ���������� ����������� �����
          }
			} // � ��������� ������ ���, ������� ���� � ������
		} else {
			if (!bit1) { // ������������ �������
				curr |= 0x80;
			} else { // ��� �� ����� �� ������ - ��������� ��������
				return 0;
			}
		}
      
		OW_WriteBit(curr & 0x80);
    
		if (!bp) {
			*addr = curr;
			curr = 0;
			addr++;
			prev = *addr;
			bp = 8;
		} else {
			prev >>= 1;
			curr >>= 1;
		}
      bp--;
	}
	lastfork = fork;
  return (1);  
}


void OW_Search(void) {
  lastfork = 65;
  for (uint8_t i = 0; i < 2; i++) {
    uint8_t p = OW_Enumerate(ow_devices[i].addr);
    if (!p) break;
  }
  
}


ow_device_t* Get_OwDevices(void) {
  return ow_devices;
}
