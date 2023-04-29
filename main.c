#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"

#define LED_PIN 5

void clock_init();
void delay_ms(uint32_t milliseconds);

volatile uint32_t ticks;

void main(void)
{
  clock_init();
  SystemCoreClockUpdate(); // Update the internal clock frequency variable

  RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOAEN_Pos);
  
  // do two dummy reads after enabling the peripheral clock, as per the errata
   volatile uint32_t dummy;
   dummy = RCC->AHB1ENR;
   dummy = RCC->AHB1ENR;

  GPIOA->MODER |= (1 << GPIO_MODER_MODER5_Pos);

  SysTick_Config(100000);
  __enable_irq();

  while(1)
  {
    GPIOA->ODR ^= (1 << LED_PIN);
    printf("[%d] Hello, World!\r\n", ticks);
    delay_ms(500);
  }
}

void clock_init()
{
  /* By default HSI (16 MHz RC oscillator) is selected as system clock.
   * We want to use the HSE (8 MHz MCO from ST-LINK connected to OSC_IN)
   * through the PLL to get 100 MHz system clock.
   */ 

  // Enable power controller and set voltage scale mode 1
  RCC->APB1ENR |= RCC_APB1ENR_PWREN_Msk;
  volatile uint32_t dummy;
  dummy = RCC->APB1ENR;
  dummy = RCC->APB1ENR;
  PWR->CR |= (0b11 << PWR_CR_VOS_Pos);

  // Configure flash controller for 3V3 supply and 100 MHz -> 3 wait states
  FLASH->ACR |= FLASH_ACR_LATENCY_3WS;

  // Set HSE bypass (to use external clock on OSC_IN, not a crystal) and enable HSE
  RCC->CR |= RCC_CR_HSEBYP_Msk | RCC_CR_HSEON_Msk;
  while (!(RCC->CR & RCC_CR_HSERDY_Msk));
  
  // Configure PLL dividers and multiplier
  /* Input to PLL should be 1-2 MHz (preferably 2 MHz). Choosing M=4 gives
   * us 8 MHz / 4 = 2 MHz.
   * The output of the PLL should be 100-438 MHz, so setting the feedback
   * multiplier to 200 gives us 2 MHz * 200 = 400 MHz.
   * The system clock should be 100 MHz. Choosing P=4 gives us
   * 400 MHz / 4 = 100 MHz
   * 
   * Since APB1 clock must not be more than 50 MHz, set the PPRE1 divider to 2.
  */
  // Clear PLLM, PLLN and PLLP bits
  RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM_Msk | RCC_PLLCFGR_PLLN_Msk | RCC_PLLCFGR_PLLP_Msk);
  
  // Set PLLM, PLLN and PLLP, and select HSE as PLL source
  RCC->PLLCFGR |= ((4 << RCC_PLLCFGR_PLLM_Pos) | (200 << RCC_PLLCFGR_PLLN_Pos) | (1 << RCC_PLLCFGR_PLLP_Pos) | (1 << RCC_PLLCFGR_PLLSRC_Pos));
  
  // Set APB1 prescaler to 2
  RCC->CFGR |= (0b100 << RCC_CFGR_PPRE1_Pos);
  
  // Enable PLL and wait for ready
  RCC->CR |= RCC_CR_PLLON_Msk;
  while (! (RCC->CR & RCC_CR_PLLRDY_Msk));

  // Select PLL output as system clock
  RCC->CFGR |= (RCC_CFGR_SW_PLL << RCC_CFGR_SW_Pos);
  while (! (RCC->CFGR & RCC_CFGR_SWS_PLL));
}

void systick_handler()
{
  ticks++;
}

void delay_ms(uint32_t milliseconds)
{
  uint32_t start = ticks;
  uint32_t end = start + milliseconds;

  if (end < start) // overflow
  {
    while (ticks > start); // wait for ticks to wrap
  }

  while (ticks < end);
}