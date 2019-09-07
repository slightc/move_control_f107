#ifndef PERIPHE_IIC
#define PERIPHE_IIC

#include "stm32f1xx.h"
#include <stm32f1xx_hal.h>

#define GET_IIC1_HANDLE() get_iic1_handle()
#define BSP_IIC1_INIT() iic1_init()

I2C_HandleTypeDef *get_iic1_handle();
void iic1_init();

#endif