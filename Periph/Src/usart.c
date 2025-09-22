/**
  ******************************************************************************
  * @file           : usart.c
  * @brief          : This file contains the common defines for the USART 
  *                   initialization functions.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
 

  /* Includes ------------------------------------------------------------------*/
#include "led.h"

/* Global variables ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


 /**
  * @brief  Initializes the corresponding USART peripheral. 
  *         The pin is configured as:
  *           - output, push-pull
  *           - low speed (2 MHz)
  *           - no pull-up and no pull-down
  * @param  port: pointer to the USART port instance
  * @retval (int) Status of operation (0 = success)
  */
int USART_Init(USART_TypeDef* port) {

  if (port == USART1) {

    MODIFY_REG(GPIOA->CRH, (GPIO_PIN_9_Mask | GPIO_PIN_10_Mask), (((_IOS_50 | _AF_PP) << (GPIO_PIN_9 - 8) * 4) | (_IN_FL << (GPIO_PIN_10 - 8) * 4)));

    #define USART1_BAUDRATE         115200
    #define USART1_FRACTION         0x0001 /* If baudrate = 9600 -> Fractual = 0x0120 */
    #define USART1_BAUDRATE_CALC    ((configCPU_CLOCK_HZ / (USART1_BAUDRATE * 16)) << 4) | USART1_FRACTION

    USART1->BRR = USART1_BAUDRATE_CALC;
    MODIFY_REG(USART1->CR1, ~(USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_UE), (USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_UE));
    
    /* USART1_IRQn interrupt configuration */
    NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
    NVIC_EnableIRQ(USART1_IRQn);

    return 0;
  }

  return 1;
}
