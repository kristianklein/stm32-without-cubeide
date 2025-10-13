#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "usart.h"
#include "stm32f4xx_hal.h"

#define LED_PIN GPIO_PIN_5
#define LED_PORT GPIOA

void clock_init();

void main(void)
{
  HAL_Init();
  clock_init();
  SystemCoreClockUpdate(); // Update the internal clock frequency variable

  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef gpio_init = {
    .Pin = LED_PIN,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_LOW,
    .Alternate = 0
  };

  HAL_GPIO_Init(LED_PORT, &gpio_init);

  usart_init(USART2);
  
  while(1)
  {
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    printf("[%.3f] Hello, World!\r\n", HAL_GetTick()/1000.0f);
    HAL_Delay(500);
  }
}

void clock_init()
{
  /* By default HSI (16 MHz RC oscillator) is selected as system clock.
   * We want to use the HSE (8 MHz MCO from ST-LINK connected to OSC_IN)
   * through the PLL to get 100 MHz system clock.
   */ 

  // Enable power controller and set voltage scale mode 1
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  // Configure PLL dividers and multiplier
  /* Input to PLL should be 1-2 MHz (preferably 2 MHz). Choosing M=4 gives
   * us 8 MHz / 4 = 2 MHz.
   * The output of the PLL should be 100-438 MHz, so setting the feedback
   * multiplier to N=200 gives us 2 MHz * 200 = 400 MHz.
   * The system clock should be 100 MHz. Choosing P=4 gives us
   * 400 MHz / 4 = 100 MHz
  */
  RCC_OscInitTypeDef osc_init = {0};
  osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  osc_init.HSEState = RCC_HSE_BYPASS;
  osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  osc_init.PLL.PLLState = RCC_PLL_ON;
  osc_init.PLL.PLLM = 4;
  osc_init.PLL.PLLN = 200;
  osc_init.PLL.PLLP = RCC_PLLP_DIV4;
  osc_init.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&osc_init) != HAL_OK)
  {
    while(1);
  }

  /* Set PLL output as the source for the system clock.
   * Since APB1 clock must not be more than 50 MHz, set the PCKL1 divider to 2.
   */
  RCC_ClkInitTypeDef clock_init = {0};
  clock_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_HCLK;
  clock_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clock_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clock_init.APB1CLKDivider = RCC_HCLK_DIV2;
  clock_init.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&clock_init, FLASH_LATENCY_3) != HAL_OK) // Configure flash controller for 3V3 supply and 100 MHz -> 3 wait states
  {
    while(1);
  }
}

void SysTick_Handler()
{
  HAL_IncTick();
}