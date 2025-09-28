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

/* Private function prototypes -----------------------------------------------*/
__STATIC_INLINE int OneWire_ErrorHandler(void);

__STATIC_INLINE void OneWire_WriteBit(uint8_t);

static void oneWireBusConfigurationTask(void* parameters);


/* Private defines -----------------------------------------------------------*/
#define _delay_ms vTaskDelay

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#define NUM_DEVICES_ON_BUS 16
static uint8_t lastfork;
static OneWireDevice_t oneWireDevices[NUM_DEVICES_ON_BUS];





/*******************************************************************************/

// -------------------------------------------------------------
void OneWireBusConfigurationInit(void) {
  
  static StaticTask_t oneWireBusConfigurationTaskTCB;
  static StackType_t oneWireBusConfigurationTaskStack[configMINIMAL_STACK_SIZE];
  
  (void) xTaskCreateStatic(
    oneWireBusConfigurationTask,
    "OW Bus Init",
    configMINIMAL_STACK_SIZE,
    NULL,
    tskIDLE_PRIORITY + 1U,
    &(oneWireBusConfigurationTaskStack[0]),
    &(oneWireBusConfigurationTaskTCB)
  );
}




// -------------------------------------------------------------
static void oneWireBusConfigurationTask(void* parameters) {
  /* Unused parameters. */
  (void) parameters;
  
  while(1) {
    if (OneWire_Search()) {
      vTaskDelete(NULL);
    }
    vTaskDelay(60000); // Research devices in the bus minutetly
  }
}




/*******************************************************************************/

// -------------------------------------------------------------
__STATIC_INLINE uint32_t irq_lock(void) {
  uint32_t p = __get_PRIMASK();
  __disable_irq();
  return p;
}




// -------------------------------------------------------------
__STATIC_INLINE void irq_unlock(uint32_t p) {
  __set_PRIMASK(p);
  __enable_irq();
}



// -------------------------------------------------------------
int OneWire_Reset(void) {

  uint32_t p = irq_lock();

  OneWire_High;
  _delay_us(580);
  OneWire_Low;
  _delay_us(15);
  
  int i = 0;
  int status = 1;
  while (i++ < 240) {
    if (!OneWire_Level) {
      status = 0;
      break;
    }
    _delay_us(1);
  }

  /* to prevent non pulled-up pin to response */
  if (i == 1) {
    status = 1;
  } else {
    _delay_us(580 - i);
  }

  irq_unlock(p);
  return status;
}


// -------------------------------------------------------------  
__STATIC_INLINE void OneWire_WriteBit(uint8_t bit) {

  uint32_t p = irq_lock();

  OneWire_High;
  if (bit) {
    _delay_us(6);
    OneWire_Low;
    _delay_us(64);
  } else {
    _delay_us(60);
    OneWire_Low;
    _delay_us(10);
  }

  irq_unlock(p);
}


// -------------------------------------------------------------  
void OneWire_WriteByte(uint8_t byte) {
  // uint8_t _byte_ = *data;
  for (int i = 0; i < 8; i++) {
    OneWire_WriteBit(byte & 0x01);
    byte >>= 1;
  }
}


// -------------------------------------------------------------  
uint8_t OneWire_ReadBit(void) {

  uint32_t p = irq_lock();

  OneWire_High;
  _delay_us(6);
  OneWire_Low;
  _delay_us(9);
  uint8_t level = OneWire_Level;
  _delay_us(55);

  irq_unlock(p);
  
  return level;
}


// -------------------------------------------------------------  
void OneWire_ReadByte(uint8_t* data) {
  for (int i = 0; i < 8; i++) {
    *data >>= 1;
    *data |= (OneWire_ReadBit()) ? 0x80 : 0;
  }
}


// -------------------------------------------------------------  
uint8_t OneWire_CRC8(uint8_t crc, uint8_t byte) {
  // 0x8c - it is a bit reverse of OneWire polinom of 0x31
  for (uint8_t i = 0; i < 8; i++) {
		crc = ((crc ^ (byte >> i)) & 0x01) ? ((crc >> 1) ^ 0x8c) : (crc >> 1);
	}

  return crc;
}


// -------------------------------------------------------------  
int OneWire_ErrorHandler(void) {
  return (-1);
}






// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------  
// -------------------------------------------------------------







__STATIC_INLINE int OneWire_Enumerate(uint8_t* addr) {
  if (!lastfork) return (1);
  
	if (OneWire_Reset()) return (1);
  
  uint8_t bp = 7;
	uint8_t prev = *addr;
	uint8_t curr = 0;
	uint8_t fork = 0;	
	uint8_t bit0 = 0;
	uint8_t bit1 = 0;
  
	OneWire_WriteByte(SearchROM);
  
	for(uint8_t i = 1; i < 65; i++) {
    bit0 = OneWire_ReadBit();
    bit1 = OneWire_ReadBit();
    
		if (!bit0) {
      if (!bit1) {
        if (i < lastfork) {
          if (prev & 1) {
            curr |= 0x80;
					} else {
            fork = i;
					}
				} else if (i == lastfork) {
          curr |= 0x80;
        } else {
          fork = i;
        }
			}
		} else {
      if (!bit1) {
        curr |= 0x80;
			} else {
        return (1);
			}
		}
    
		OneWire_WriteBit(curr & 0x80);
    
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
  return (0);  
}


// -------------------------------------------------------------
int OneWire_Search(void) {
  if (OneWire_Reset()) return (1);
  lastfork = 65;
  for (uint8_t i = 0; i < NUM_DEVICES_ON_BUS; i++) {
    if (OneWire_Enumerate(oneWireDevices[i].addr)) break;
  }
  return (0);
}


// -------------------------------------------------------------
uint8_t OneWire_ReadPowerSupply(uint8_t* addr) {
  OneWire_MatchROM(addr);
  OneWire_WriteByte(ReadPowerSupply);
  
  return !OneWire_ReadBit();
}


/**
 * @brief   Determines the existent of the device with given address, on the bus.
 * @param   addr pointer to OneWire device address
 * @retval  (uint8_t) status of operation
 */
int OneWire_MatchROM(uint8_t* addr) {
  if (OneWire_Reset()) return 1;
  
  OneWire_WriteByte(MatchROM);
  for (uint8_t i = 0; i < 8; i++) {
    OneWire_WriteByte(addr[i]);
  }

  return 0;
}


// -------------------------------------------------------------
OneWireDevice_t* Get_OwDevices(void) {
  return oneWireDevices;
}







