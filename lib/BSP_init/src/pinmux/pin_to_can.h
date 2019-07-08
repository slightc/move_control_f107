#ifndef PINMUX_TO_CAN
#define PINMUX_TO_CAN

#include "stm32f1xx.h"
#include <stm32f1xx_hal.h>
#include "system/system_init.h"

#define PA12_PA11_TO_CAN() \
    __HAL_AFIO_REMAP_CAN1_1();\
    pinmux_to_can_tx(GPIOA,GPIO_PIN_12);\
    pinmux_to_can_rx(GPIOA,GPIO_PIN_11)

#define PB9_PB8_TO_CAN() \
    __HAL_AFIO_REMAP_CAN1_2();\
    pinmux_to_can_tx(GPIOB,GPIO_PIN_9);\
    pinmux_to_can_rx(GPIOB,GPIO_PIN_8)

#define PD1_PD0_TO_CAN() \
    __HAL_AFIO_REMAP_CAN1_3();\
    pinmux_to_can_tx(GPIOD,GPIO_PIN_1);\
    pinmux_to_can_rx(GPIOD,GPIO_PIN_0)

void pinmux_to_can_tx(GPIO_TypeDef *GPIOx,uint16_t pinx);
void pinmux_to_can_rx(GPIO_TypeDef *GPIOx,uint16_t pinx);

#endif