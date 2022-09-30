#include <stdint.h>
#include "stm32f4xx.h"

/* Required headers:
  * - stm32f4xx.h
  *  - stm32f410rx.h
  *     - core_cm4.h
  *       - cmsis_version.h
  *       - cmsis_compiler.h
  *         - cmsis_gcc.h
  *       - mpu_armv7.h
  *     - system_stm32f4xx.h (system_stm32f4xx.c)
 */

#define LED_PIN 5

void clock_init();
void peripheral_enable_wait(volatile uint32_t *register_address);

void main(void)
{
  /*  Clock setup:
   *  By default the NUCLEO board uses the 8 MHz MCO from the ST-LINK as HSE.
   *  Also has on-board 32 kHz LSE (X2).
   */

  clock_init();

  RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOAEN_Pos);
  
  // // do two dummy reads after enabling the peripheral clock, as per the errata
   volatile uint32_t dummy;
   dummy = RCC->AHB1ENR;
   dummy = RCC->AHB1ENR;

  GPIOA->MODER |= (1 << GPIO_MODER_MODER5_Pos);
  
  while(1)
  {
    GPIOA->ODR ^= (1 << LED_PIN);
    for (uint32_t i = 0; i < 1000000; i++);
  }
}

void clock_init()
{
  /* By default HSI (16 MHz RC oscillator) is selected as system clock.
   * We want to use the HSE (8 MHz MCO from ST-LINK connected to OSC_IN)
   * through the PLL to get 100 MHz system clock.
   */ 

  // Set the voltage scaling to VOS1 to allow 100 MHz clock
  RCC->APB1ENR |= RCC_APB1ENR_PWREN_Msk;
  volatile uint32_t dummy;
  dummy = RCC->APB1ENR;
  dummy = RCC->APB1ENR;
  // peripheral_enable_wait(&RCC->APB1ENR);
  PWR->CR |= (0b11 << PWR_CR_VOS_Pos); // VOS1

  // Configure flash controller for 100 MHz (3.3V -> 3 wait states)
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
  // Clear PLLM, PLLN and PLLP bits first
  RCC->PLLCFGR &= ~((0b111111 << RCC_PLLCFGR_PLLM_Pos) | (0b111111111 << RCC_PLLCFGR_PLLN_Pos) | (0b11 << RCC_PLLCFGR_PLLP_Pos));
  
  // Set PLLM, PLLN and PLLP, and select HSE as PLL source
  RCC->PLLCFGR |= ((4 << RCC_PLLCFGR_PLLM_Pos) | (200 << RCC_PLLCFGR_PLLN_Pos) | (1 << RCC_PLLCFGR_PLLP_Pos) | (1 << RCC_PLLCFGR_PLLSRC_Pos));
  RCC->CFGR |= (0b100 << RCC_CFGR_PPRE1_Pos); // APB1 prescaler
  
  // Enable PLL and wait for ready
  RCC->CR |= RCC_CR_PLLON_Msk;
  while (! (RCC->CR & RCC_CR_PLLRDY_Msk));

  // Select HSE as system clock
  RCC->CFGR |= (RCC_CFGR_SW_PLL << RCC_CFGR_SW_Pos);
  while (! (RCC->CFGR & RCC_CFGR_SWS_PLL));
}

void peripheral_enable_wait(volatile uint32_t *register_address)
{
  volatile uint32_t dummy;
  dummy = *register_address;
  dummy = *register_address;
}

void hard_fault_handler()
{
  while(1);
}