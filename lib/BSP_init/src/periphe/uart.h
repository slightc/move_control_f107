#ifndef PERIPHE_UART
#define PERIPHE_UART

#include "stm32f1xx.h"
#include <stm32f1xx_hal.h>
#include "cmsis_os.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define GET_UART1_HANDLE() get_uartx_handle(USART1)
#define BSP_UART1_INIT() \
    __HAL_RCC_USART1_CLK_ENABLE();\
    uart_init(USART1,115200)
#define GET_UART2_HANDLE() get_uartx_handle(USART2)
#define BSP_UART2_INIT() \
    __HAL_RCC_USART2_CLK_ENABLE();\
    uart_init(USART2,115200)

UART_HandleTypeDef* get_uartx_handle(USART_TypeDef *UARTx);
uint8_t uart_init(USART_TypeDef *UARTx, uint32_t baud_rate);
uint8_t get_uart_rx_count(USART_TypeDef *UARTx);
uint8_t uart_read(USART_TypeDef *UARTx, uint8_t *buffer, uint8_t len, uint32_t timeout);
void uart_print(USART_TypeDef *UARTx, uint8_t *str, int32_t data, int8_t mode);
void uart_printf(USART_TypeDef *UARTx, char *str, ...);

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);

#endif