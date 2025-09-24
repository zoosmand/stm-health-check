/**
  ******************************************************************************
  * @file           : common.c
  * @brief          : Common used routines and printf() supply
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
 
 
/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Private function prototypes -----------------------------------------------*/
__STATIC_INLINE uint32_t ITM_SendCharChannel(uint32_t ch, uint32_t channel);
__STATIC_INLINE void _putc(uint8_t ch);


////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void) {
  while (1) {
    //
  }
}




/********************************************************************************/
/*                         printf() output supply block                         */
/********************************************************************************/
/**
  * @brief  Sends a symbol into ITM channel. It could be cought with SWO pin on an MC. 
  * @param ch: a symbol to be output
  * @param channel: number of an ITM channel
  * @retval the same symbol 
  */
__STATIC_INLINE uint32_t ITM_SendCharChannel(uint32_t ch, uint32_t channel) {
   /* ITM enabled and ITM Port enabled */
  if (((ITM->TCR & ITM_TCR_ITMENA_Msk) != 0UL) && ((ITM->TER & (1 << channel)) != 0UL)) {
    while (ITM->PORT[channel].u32 == 0UL) {
      __NOP();
    }
    ITM->PORT[channel].u8 = (uint8_t)ch;
  }
  return (ch);
}




/**
  * @brief  Sends a symbol into USART. 
  * @param device: a pointer USART_TypeDef
  * @param ch: a symbol to be output
  * @param check: a pointer to a BitBand check bit
  * @retval none: 
  */
__STATIC_INLINE void _putc(uint8_t ch) {
  if (ch == '\n') _putc('\r');

  #ifdef SWO_ITM
    ITM_SendCharChannel(ch, SWO_ITM);
 #endif

 #ifdef SWO_DSPL
    PrintCharDisplay(ch, SWO_DSPL);
 #endif

 #ifdef SWO_USART
    while (!(PREG_CHECK(SWO_USART->SR, USART_SR_TXE_Pos)));
    SWO_USART->DR = ch;
  #endif
}




/**
  * @brief An interpretation of the __weak system _write()
  * @param file: IO file
  * @param ptr: pointer to a char(symbol) array
  * @param len: length oa the array
  * @retval length of the array 
  */
int _write(int32_t file, char *ptr, int32_t len) {
  for(int32_t i = 0 ; i < len ; i++) {
    _putc(*ptr++);  
  }
	return len;
}




#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr If expr is false, it calls assert_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed.
  *         If expr is true, it returns no value.
  * @retval None
  */
#define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))

/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t *file, uint32_t line);
#else
#define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */




__STATIC_INLINE void _DWT_Init(void) {
  // SET_BIT(CoreDebug->DEMCR, CoreDebug_DEMCR_TRCENA_Msk);
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCEVTENA_Msk | DWT_CTRL_CYCCNTENA_Msk;
  __DSB();
  __ISB();
}



void _delay_us(uint32_t us) {
  _DWT_Init();
  uint32_t const start = DWT->CYCCNT;
  uint32_t const ticks = us * (configCPU_CLOCK_HZ / 1000000U);
  while ((READ_REG(DWT->CYCCNT) - start) < ticks) { __asm volatile("nop"); }
  DWT->CTRL &= ~(DWT_CTRL_CYCEVTENA_Msk | DWT_CTRL_CYCCNTENA_Msk);
}