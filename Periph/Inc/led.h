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
int LED_Init(GPIO_TypeDef*, uint16_t);


#ifdef __cplusplus
}
#endif

#endif /* __LED_INIT_H */