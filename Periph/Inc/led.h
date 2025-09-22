/**
  ******************************************************************************
  * @file           : led.h
  * @brief          : Header for led.c file.
  *                   This file contains the common defines for the GPIO
  *                   initialization functions.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
 
  /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_INIT_H
#define __LED_INIT_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Exported functions prototypes ---------------------------------------------*/

/**
  * @brief  Initializes the corresponding LED pin on the board. 
  *         The pin is configured as:
  *           - output, push-pull
  *           - low speed (2 MHz)
  *           - no pull-up and no pull-down
  * @param  port: pointer to the GPIO port instance
  * @param  pin:  pin number (0..15)
  * @retval (int) Status of operation (0 = success)
  */
int LED_Init(GPIO_TypeDef*, uint16_t);


#ifdef __cplusplus
}
#endif

#endif /* __LED_INIT_H */