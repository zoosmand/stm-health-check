/**
  ******************************************************************************
  * @file           : ds18b20.h
  * @brief          : Header for ds18b20.c file.
  *                   This file contains the common defines for DS18B20
  *                   temperature measurment service.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
 
  /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DS18B20_H
#define __DS18B20_H

#ifdef __cplusplus
  extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "onewire.h"


/* Exported functions prototypes ---------------------------------------------*/

/**
  * @brief  Temperature measurement service based on OneWire devices
  * @param  none
  * @retval none
 */
void TemperatureMeasurmentService(void);


#ifdef __cplusplus
}
#endif

#endif /* __DS18B20_H */