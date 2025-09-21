/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <timers.h>
#include <semphr.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "misc.h"
#include "stm32f103xb.h"

/* Private includes ----------------------------------------------------------*/
#include "common.h"
#include "stm32f10x_it.h"

/* Exported constants --------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
// #define SWO_ITM     0
// #define SWO_DSPL    0
#define SWO_USART   USART1


/* Exported functions prototypes ---------------------------------------------*/
extern void Set_BitBandVal(uint32_t, uint32_t);
extern uint32_t Get_BitBandVal(uint32_t);



#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

