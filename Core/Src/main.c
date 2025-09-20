/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Global variables ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void exampleTask( void * parameters );

int LED_Init(GPIO_TypeDef*, uint16_t);



////////////////////////////////////////////////////////////////////////////////

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void) {

  static StaticTask_t exampleTaskTCB;
  static StackType_t exampleTaskStack[ configMINIMAL_STACK_SIZE ];

  ( void ) printf( "Example FreeRTOS Project\n" );

  ( void ) xTaskCreateStatic( exampleTask,
                              "example",
                              configMINIMAL_STACK_SIZE,
                              NULL,
                              configMAX_PRIORITIES - 1U,
                              &( exampleTaskStack[ 0 ] ),
                              &( exampleTaskTCB ) );


  /* Start the scheduler. */
  vTaskStartScheduler();

  while (1);
}



#if ( configCHECK_FOR_STACK_OVERFLOW > 0 )

    void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                        char * pcTaskName )
    {
        /* Check pcTaskName for the name of the offending task,
         * or pxCurrentTCB if pcTaskName has itself been corrupted. */
        ( void ) xTask;
        ( void ) pcTaskName;
    }

#endif /* #if ( configCHECK_FOR_STACK_OVERFLOW > 0 ) */





/*-----------------------------------------------------------*/
static void exampleTask( void * parameters ) {
    /* Unused parameters. */
    ( void ) parameters;

    while(1) {
        /* Example Task Code */
        LED_Blink(GPIOC, GPIO_PIN_13);
        vTaskDelay( 100 ); /* delay 100 ticks */
    }
}













/**
  * @brief  Setup the microcontroller system
  *         Initialize the Embedded Flash Interface, the PLL and update the 
  *         SystemCoreClock variable.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
void SystemInit (void) {

  #if (PREFETCH_ENABLE != 0)
    PREG_SET(FLASH->ACR, FLASH_ACR_PRFTBE_Pos);
  #else
    PREG_CLR(FLASH->ACR, FLASH_ACR_PRFTBE_Pos);
  #endif /* PREFETCH_ENABLE */

  /* SysCfg */
  PREG_SET(RCC->APB2ENR, RCC_APB2ENR_AFIOEN_Pos);
  while (!(PREG_CHECK(RCC->APB2ENR, RCC_APB2ENR_AFIOEN_Pos)));

  /* PWR */
  PREG_SET(RCC->APB1ENR, RCC_APB1ENR_PWREN_Pos);
  while (!(PREG_CHECK(RCC->APB1ENR, RCC_APB1ENR_PWREN_Pos)));

  /* Flash */
  MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_1);
  if (READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_1) {
    Error_Handler();
  }

  /* JTAG-DP disabled and SW-DP enabled */
  CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_SWJ_CFG);
  SET_BIT(AFIO->MAPR, AFIO_MAPR_SWJ_CFG_JTAGDISABLE);

  /* HSE enable and wait until it runs */
  PREG_SET(RCC->CR, RCC_CR_HSEON_Pos);
  while (!(PREG_CHECK(RCC->CR, RCC_CR_HSERDY_Pos)));

  /* LSI enable and wait until it runs */
  PREG_SET(RCC->CSR, RCC_CSR_LSION_Pos);
  while (!(PREG_CHECK(RCC->CR, RCC_CSR_LSIRDY_Pos)));

  /* Enable backup registers access */
  PREG_SET(PWR->CR, PWR_CR_DBP_Pos);

  /* Force backup domain reset */
  PREG_SET(RCC->BDCR, RCC_BDCR_BDRST_Pos);
  PREG_CLR(RCC->BDCR, RCC_BDCR_BDRST_Pos);

  /* LSE enable and wait until it runs */
  PREG_SET(RCC->BDCR, RCC_BDCR_LSEON_Pos);
  while (!(PREG_CHECK(RCC->CR, RCC_BDCR_LSERDY_Pos)));

  /* RTC Source is LSE */
  MODIFY_REG(RCC->BDCR, RCC_BDCR_RTCSEL, RCC_BDCR_RTCSEL_0);

  /* Enable RTC */
  PREG_SET(RCC->BDCR, RCC_BDCR_RTCEN_Pos);

  /* PLL confugure domain clock */
  RCC->CFGR |= RCC_CFGR_PLLMULL9; // mutiprexing pll on 9
  PREG_SET(RCC->CFGR, RCC_CFGR_PLLSRC_Pos); // PLL is the clock source

  /* PLL enable and wait until it runs */
  PREG_SET(RCC->CR, RCC_CR_PLLON_Pos);
  while (!(PREG_CHECK(RCC->CR, RCC_CR_PLLRDY_Pos)));

  /* AHB clock isn't divided */
  /* APB1 clock divided by 2 */
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV2);
  /* APB2 clock isn't divided */

  /* set PLL as sysclock source and wait until it runs */
  MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
  while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);






  /*----------------------------------------------------------------------------*/
  /*                          Set peripheral clocks                             */
  /*----------------------------------------------------------------------------*/
  /* AHB peripherals */
  SET_BIT(RCC->AHBENR, (
      RCC_AHBENR_DMA1EN
    | RCC_AHBENR_CRCEN
    | RCC_AHBENR_SRAMEN
  ));

  /* APB1 peripherals */
  // SET_BIT(RCC->APB1ENR, (
  //   RCC_APB1ENR_TIM7EN
  // ));

  /* APB2 peripherals */
  SET_BIT(RCC->APB2ENR, (
      RCC_APB2ENR_IOPAEN
    | RCC_APB2ENR_IOPCEN
    | RCC_APB2ENR_IOPEEN
    | RCC_APB2ENR_USART1EN
    | RCC_APB2ENR_SPI1EN
  ));


  
  
  /* Stop ticking peripheral while debugging */
  #if (DEBUG != 0)
    SET_BIT(DBGMCU->CR , (
        DBGMCU_CR_DBG_IWDG_STOP
      | DBGMCU_CR_DBG_WWDG_STOP
    ));
  #endif /* DEBUG */


  LED_Init(GPIOC, GPIO_PIN_13);

}



/**
  * @brief  Initiates PIN 6 and 7 on GPIOA port as outputs to blink correspondent LEDs
  *  on the board. Pins are set on the lowest speed, push-pull, no pull-up neither pull-down
  * @param  None
  * @return None
  */
int LED_Init(GPIO_TypeDef* port, uint16_t pinSource) {

  pinSource = (pinSource >= 8) ? (pinSource - 8)*4 : pinSource*4;

  MODIFY_REG(port->CRH, 0xf << pinSource, _IOS_2 << pinSource);

  return 0;
}
