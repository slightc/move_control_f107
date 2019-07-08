#ifndef PERIPHE_CAN
#define PERIPHE_CAN

#include "stm32f1xx.h"
#include <stm32f1xx_hal.h>

#define CAN1_INIT() can1_init()
#define CAN1_HANDLE() get_can1_handle()

CAN_HandleTypeDef *get_can1_handle();
void can1_init();
void can1_filter_init();

#endif