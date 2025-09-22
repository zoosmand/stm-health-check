/**
  ******************************************************************************
  * @file           : led.c
  * @brief          : This file contains the common defines for the GPIO
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
  * @brief  Initializes the corresponding LED pin on the board. 
  *         The pin is configured as:
  *           - output, push-pull
  *           - low speed (2 MHz)
  *           - no pull-up and no pull-down
  * @param  port: pointer to the GPIO port instance
  * @param  pin:  pin number (0..15)
  * @retval (int) Status of operation (0 = success)
  */
int LED_Init(GPIO_TypeDef *port, uint16_t pin) {
    uint32_t shift;
    #define GPIO_MODE_OUTPUT_PP_2MHZ (_IOS_2 | _GPO_PP)

    if (pin < 8) {
        shift = pin * 4;
        MODIFY_REG(port->CRL, (0xf << shift), (GPIO_MODE_OUTPUT_PP_2MHZ << shift));
    } else {
        shift = (pin - 8) * 4;
        MODIFY_REG(port->CRH, (0xf << shift), (GPIO_MODE_OUTPUT_PP_2MHZ << shift));
    }

    return 0;
}

