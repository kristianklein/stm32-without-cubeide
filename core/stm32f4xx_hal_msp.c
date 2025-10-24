/**
  ******************************************************************************
  * @file    stm32f4xx_hal_msp_template.c
  * @author  MCD Application Team
  * @brief   This file contains the HAL System and Peripheral (PPP) MSP initialization
  *          and de-initialization functions.
  *          It should be copied to the application folder and renamed into 'stm32f4xx_hal_msp.c'.           
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @defgroup HAL_MSP HAL MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions HAL MSP Private Functions
  * @{
  */

/**
  * @brief  Initializes the Global MSP.
  * @note   This function is called from HAL_Init() function to perform system
  *         level initialization (GPIOs, clock, DMA, interrupt).
  * @retval None
  */
void HAL_MspInit(void)
{

}

/**
  * @brief  DeInitializes the Global MSP.
  * @note   This functiona is called from HAL_DeInit() function to perform system
  *         level de-initialization (GPIOs, clock, DMA, interrupt).
  * @retval None
  */
void HAL_MspDeInit(void)
{

}

/**
  * @brief  Initializes the PPP MSP.
  * @note   This functiona is called from HAL_PPP_Init() function to perform 
  *         peripheral(PPP) system level initialization (GPIOs, clock, DMA, interrupt)
  * @retval None
  */
void HAL_PPP_MspInit(void)
{

}

/**
  * @brief  DeInitializes the PPP MSP.
  * @note   This functiona is called from HAL_PPP_DeInit() function to perform 
  *         peripheral(PPP) system level de-initialization (GPIOs, clock, DMA, interrupt)
  * @retval None
  */
void HAL_PPP_MspDeInit(void)
{

}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2)
  {
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    GPIO_InitTypeDef rx_tx_init = {0};
    rx_tx_init.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    rx_tx_init.Mode = GPIO_MODE_AF_PP;
    rx_tx_init.Alternate = GPIO_AF7_USART2;
    rx_tx_init.Pull = GPIO_NOPULL;
    rx_tx_init.Speed = GPIO_SPEED_FAST;

    HAL_GPIO_Init(GPIOA, &rx_tx_init);
  }
}