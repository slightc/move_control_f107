#ifndef PERIPHE_UART
#define PERIPHE_UART

#include "stm32f1xx.h"
#include <stm32f1xx_hal.h>

#define GET_UART1_HANDLE() get_uartx_handle(USART1)
#define BSP_UART1_INIT() \
    __HAL_RCC_USART1_CLK_ENABLE();\
    uart_init(USART1,115200)

UART_HandleTypeDef* get_uartx_handle(USART_TypeDef *UARTx);
uint8_t uart_init(USART_TypeDef *UARTx, uint32_t baud_rate);
uint8_t uart_read(UART_HandleTypeDef *phuart, uint8_t *buffer, uint8_t len, uint32_t timeout);

#endif