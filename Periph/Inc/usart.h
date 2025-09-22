/**
  ******************************************************************************
  * @file           : usart.h
  * @brief          : Header for usart.c file.
  *                   This file contains the common defines for the USART
  *                   initialization functions.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
 
  /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_INIT_H
#define __USART_INIT_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Exported functions prototypes ---------------------------------------------*/
int USART_Init(USART_TypeDef*);


#ifdef __cplusplus
}
#endif

#endif /* __USART_INIT_H */