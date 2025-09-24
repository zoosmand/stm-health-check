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
  * @retval none
  */
static void temperatureMeasurement_Workflow(void); 




/*******************************************************************************/

void TemperatureMeasurmentService(void) {

  static StaticTask_t temperatureMeasurementTaskTCB;
  static StackType_t temperatureMeasurementTaskStack[configMINIMAL_STACK_SIZE * 4];

  (void) xTaskCreateStatic(
                            temperatureMeasurementTask,
                            "Temp Meas",
                            configMINIMAL_STACK_SIZE * 4,
                            NULL,
                            tskIDLE_PRIORITY + 1,
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
  // OneWire_Search();
  OneWireDevice_t* devs = Get_OwDevices();
  printf("test\n");
  vTaskDelay(10000);
}


