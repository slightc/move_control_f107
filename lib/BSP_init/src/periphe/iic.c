#include "can.h"

I2C_HandleTypeDef h_iic1;

I2C_HandleTypeDef *get_iic1_handle()
{
    return &h_iic1;
}

void iic1_init()
{
    I2C_HandleTypeDef *p_iic1 = get_iic1_handle();
    __HAL_RCC_I2C1_CLK_ENABLE();

	p_iic1->Instance = I2C1;

    p_iic1->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    p_iic1->Init.ClockSpeed = 100000;
    p_iic1->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    p_iic1->Init.DutyCycle = I2C_DUTYCYCLE_2;
    p_iic1->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    p_iic1->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    p_iic1->Init.OwnAddress1 = 0x01;
    p_iic1->Init.OwnAddress2 = 0xFE;

    HAL_I2C_Init(p_iic1);
}