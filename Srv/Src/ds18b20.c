/**
  ******************************************************************************
  * @file           : ds18b20.c
  *                   This file contains OneWire DS18B20 temperature sensor
  *                   code. 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
 

/* Includes ------------------------------------------------------------------*/
#include "ds18b20.h"

/* Global variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void temperatureMeasurementTask(void* parameters);

/**
  * @brief  Temperature measurement workflow
  * @param  port: pointer to the GPIO port instance
  * @param  pin:  pin number (0..15)
  * @param  callbackDelay:  pointer to delay function 
  * @param  delay:  delay value 
  * @retval none
  */
static void temperatureMeasurement_Workflow(void); 




/*******************************************************************************/

void TemperatureMeasurmentService(void) {

  static StaticTask_t temperatureMeasurementTaskTCB;
  static StackType_t temperatureMeasurementTaskStack[configMINIMAL_STACK_SIZE];

  (void) xTaskCreateStatic(
                            temperatureMeasurementTask,
                            "Temperature Measurment",
                            configMINIMAL_STACK_SIZE,
                            NULL,
                            configMAX_PRIORITIES - 1U,
                            &(temperatureMeasurementTaskStack[0]),
                            &(temperatureMeasurementTaskTCB)
                          );
}



static void temperatureMeasurementTask(void* parameters) {
  /* Unused parameters. */
  (void) parameters;

  while(1) {
      temperatureMeasurement_Workflow();
  }
}




static void temperatureMeasurement_Workflow(void) {
  OW_Reset();
  vTaskDelay(10000);
}


