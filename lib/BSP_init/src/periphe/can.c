#include "can.h"

CAN_HandleTypeDef h_can1;

CAN_HandleTypeDef *get_can1_handle()
{
    return &h_can1;
}

void can1_filter_init()
{
    CAN_FilterTypeDef can1_filter;
    CAN_HandleTypeDef *p_can1 = get_can1_handle();

	can1_filter.FilterBank = 0;
	can1_filter.FilterMode = CAN_FILTERMODE_IDMASK;
	can1_filter.FilterScale = CAN_FILTERSCALE_32BIT;
	can1_filter.FilterIdHigh = 0x0000;
	can1_filter.FilterIdLow = 0x0000;
	can1_filter.FilterMaskIdHigh = 0x0000;
	can1_filter.FilterMaskIdLow = 0x0000;
	can1_filter.FilterFIFOAssignment = 0;
	can1_filter.FilterActivation = ENABLE;

    HAL_CAN_ConfigFilter(p_can1,&can1_filter);
}

void can1_init()
{
    CAN_HandleTypeDef *p_can1 = get_can1_handle();
    __HAL_RCC_CAN1_CLK_ENABLE();



    p_can1->Instance = CAN1;

    p_can1->Init.TimeTriggeredMode      = DISABLE;
	p_can1->Init.AutoBusOff             = DISABLE;
	p_can1->Init.AutoWakeUp             = DISABLE;
	p_can1->Init.AutoRetransmission     = DISABLE;
	p_can1->Init.ReceiveFifoLocked      = DISABLE;
	p_can1->Init.TransmitFifoPriority   = ENABLE;
	p_can1->Init.Mode           = CAN_MODE_NORMAL;
	p_can1->Init.SyncJumpWidth  = CAN_SJW_1TQ;
	p_can1->Init.TimeSeg1       = CAN_BS1_5TQ;
	p_can1->Init.TimeSeg2       = CAN_BS2_3TQ;
	p_can1->Init.Prescaler      = ((SystemCoreClock / 2 / 1000000) / (1 + 5 + 3));
    HAL_CAN_Init(p_can1);
    can1_filter_init();
	
	HAL_CAN_Start(p_can1);
}


void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
}