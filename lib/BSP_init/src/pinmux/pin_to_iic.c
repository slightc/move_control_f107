#include "pin_to_iic.h"

void pinmux_to_iic_scl(GPIO_TypeDef *GPIOx,uint16_t pinx)
{
    GPIO_InitTypeDef scl_gpio;

    enable_GPIOx_clock(GPIOx);
    scl_gpio.Pin   = pinx;
    scl_gpio.Mode  = GPIO_MODE_AF_OD;
    scl_gpio.Pull  = GPIO_PULLUP;
    scl_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOx, &scl_gpio);
}

void pinmux_to_iic_sda(GPIO_TypeDef *GPIOx,uint16_t pinx)
{
    GPIO_InitTypeDef sda_gpio;

    enable_GPIOx_clock(GPIOx);
    sda_gpio.Pin   = pinx;
    sda_gpio.Mode  = GPIO_MODE_AF_OD;
    sda_gpio.Pull  = GPIO_PULLUP;
    sda_gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOx, &sda_gpio);
}