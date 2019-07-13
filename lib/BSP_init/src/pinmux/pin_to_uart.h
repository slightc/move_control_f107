#ifndef PINMUX_TO_UART
#define PINMUX_TO_UART

#include "stm32f1xx.h"
#include <stm32f1xx_hal.h>
#include "system/system_init.h"

#define PB6_PB7_TO_UART1() \
    __HAL_AFIO_REMAP_USART1_ENABLE();\
    pinmux_to_uart_tx(GPIOB,GPIO_PIN_6);\
    pinmux_to_uart_rx(GPIOB,GPIO_PIN_7)

#define PD5_PD6_TO_UART2() \
    __HAL_AFIO_REMAP_USART2_ENABLE();\
    pinmux_to_uart_tx(GPIOD,GPIO_PIN_5);\
    pinmux_to_uart_rx(GPIOD,GPIO_PIN_6)

#define PD8_PD9_TO_UART3() \
    __HAL_AFIO_REMAP_USART3_ENABLE();\
    pinmux_to_uart_tx(GPIOD,GPIO_PIN_8);\
    pinmux_to_uart_rx(GPIOD,GPIO_PIN_9)

#define PC10_PC11_TO_UART4() \
    pinmux_to_uart_tx(GPIOC,GPIO_PIN_10);\
    pinmux_to_uart_rx(GPIOC,GPIO_PIN_11)

#define PC12_PD2_TO_UART5() \
    __HAL_AFIO_REMAP_USART2_ENABLE();\
    pinmux_to_uart_tx(GPIOC,GPIO_PIN_12);\
    pinmux_to_uart_rx(GPIOD,GPIO_PIN_2)

void pinmux_to_uart_tx(GPIO_TypeDef *GPIOx,uint16_t pinx);
void pinmux_to_uart_rx(GPIO_TypeDef *GPIOx,uint16_t pinx);

#endif