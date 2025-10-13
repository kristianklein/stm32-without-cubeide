#include "usart.h"
#include "stm32f4xx_hal.h"

UART_HandleTypeDef uart2 = {0};

void usart_init(USART_TypeDef *usart)
{
  /* Initialize UART */
  uart2.Instance = usart;
  uart2.Init.BaudRate = 115200;
  uart2.Init.Mode = UART_MODE_TX;
  uart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart2.Init.WordLength = UART_WORDLENGTH_8B;
  uart2.Init.StopBits = UART_STOPBITS_1;
  uart2.Init.Parity = UART_PARITY_NONE;
  uart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&uart2);

  // Dummy write, because the first byte seems to always be dropped
   usart->DR = 0;
   while (!(usart->SR & USART_SR_TC));
}

void usart_write(USART_TypeDef *usart, char c)
{
  if (uart2.Instance == usart)
  {
    HAL_UART_Transmit(&uart2, &c, 1, HAL_MAX_DELAY);
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2)
  {
    /* Enable USART2 and GPIOA clock */
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    /* Set PA2 and PA3 to alternate function */
    GPIO_InitTypeDef rx_tx_init = {0};
    rx_tx_init.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    rx_tx_init.Mode = GPIO_MODE_AF_PP;
    rx_tx_init.Alternate = GPIO_AF7_USART2;
    rx_tx_init.Pull = GPIO_NOPULL;
    rx_tx_init.Speed = GPIO_SPEED_FAST;

    HAL_GPIO_Init(GPIOA, &rx_tx_init);
  }
}