#include "pin_to_uart.h"

void pinmux_to_uart_tx(GPIO_TypeDef *GPIOx,uint16_t pinx){
    GPIO_InitTypeDef tx_gpio;

    enable_GPIOx_clock(GPIOx);
    tx_gpio.Pin   = pinx;
    tx_gpio.Mode  = GPIO_MODE_AF_PP;
    tx_gpio.Pull  = GPIO_PULLUP;
    tx_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOx, &tx_gpio);
}

void pinmux_to_uart_rx(GPIO_TypeDef *GPIOx,uint16_t pinx){
    GPIO_InitTypeDef rx_gpio;

    enable_GPIOx_clock(GPIOx);
    rx_gpio.Pin   = pinx;
    rx_gpio.Mode  = GPIO_MODE_AF_INPUT;
    rx_gpio.Pull  = GPIO_PULLUP;
    rx_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOx, &rx_gpio);
}