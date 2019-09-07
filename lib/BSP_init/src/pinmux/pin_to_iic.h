#ifndef PINMUX_TO_IIC
#define PINMUX_TO_IIC

#include "stm32f1xx.h"
#include <stm32f1xx_hal.h>
#include "system/system_init.h"

#define PB8_PB9_TO_IIC1() \
    __HAL_AFIO_REMAP_I2C1_ENABLE();\
    pinmux_to_iic_scl(GPIOB,GPIO_PIN_8);\
    pinmux_to_iic_sda(GPIOB,GPIO_PIN_9)

void pinmux_to_iic_scl(GPIO_TypeDef *GPIOx,uint16_t pinx);
void pinmux_to_iic_sda(GPIO_TypeDef *GPIOx,uint16_t pinx);

#endif