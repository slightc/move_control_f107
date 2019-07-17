#include "main.h"
#include "system/system_init.h"
#include "pinmux/pin_to_can.h"
#include "pinmux/pin_to_uart.h"
#include "periphe/can.h"
#include "periphe/uart.h"
#include "robomodule/can_bus.h"
#include "cmsis_os.h"
#include "stm32f1xx_it.h"
#include "aoa_uwb/aoa_uwb.h"

#define CHG_INT16_HL(n) ( ((n&0xff)<<8) |((n>>8)&0xff) )

osThreadId defaultTaskHandle;
osThreadId uartTaskHandle;
osThreadId canTaskHandle;
int16_t aoa_distance=0;
int16_t aoa_angle=0;

void led_blinkly(void const *argument){
  
    (void) argument;
	for (;;)
    {   
        HAL_GPIO_TogglePin(LED_GPIO_PORT,LED_PIN);
        osDelay(500);
    }
}

void uart_test(void const *argument){
    UART_HandleTypeDef *p_huart1;
    uint8_t send_str[] = "qweasdzxc\n";
    AOA_Report_t *p=NULL;
    int16_t distance=0;
  
    (void) argument;
    p_huart1 = GET_UART1_HANDLE();
	for (;;)
    {   
        p=wait_aoa_report_packet(100);
        // HAL_UART_Transmit(p_huart1,send_str,10,5);
        if(p==NULL)
        {
            // uart_ptint(USART1,"timeout",0,10);
        }else{
            uart_ptint(USART1,"have packet",0,10);
            aoa_distance = (int16_t)CHG_INT16_HL(p->distance);
            aoa_angle = (int16_t)CHG_INT16_HL(p->angle);
            distance = 0.98*distance + 0.02*(int16_t)CHG_INT16_HL(p->distance);
            // uart_ptint(USART1,"d:",distance,10);
            // uart_ptint(USART1,"a:",(int16_t)CHG_INT16_HL(p->angle),10);
        }
        osDelay(50);
        // for(uint8_t i=0;i<100;i++){
        //     AOA_Tag_Handler();
        // }
        // p = AOA_GetMsg();
        // uart_read(USART1,send_str,10,10000);
        // HAL_UART_Transmit(p_huart1,send_str,10,5);
        // osDelay(500);
        // uart_ptint(USART1,"d:",CHG_INT16_HL(p->distance),10);
        // uart_ptint(USART1,"a:",CHG_INT16_HL(p->angle),10);
    }
}

void can_test(void const *argument){
    CAN_HandleTypeDef *p_can1;
    CAN_RxHeaderTypeDef can1_rx_header;
    int8_t data[8],len;

    (void) argument;

    p_can1 = GET_CAN1_HANDLE();
	for (;;)
    {   
        uart_ptint(USART1,"can_test",0,10);
        len = HAL_CAN_GetRxFifoFillLevel(p_can1,CAN_FILTER_FIFO0);
        if(len!=0){
            HAL_CAN_GetRxMessage(p_can1,CAN_FILTER_FIFO0,&can1_rx_header,data);
            uart_ptint(USART1,"id ",can1_rx_header.StdId,10);
            uart_ptint(USART1,"DLC ",can1_rx_header.DLC,10);
            uart_ptint(USART1,"d0 ",data[0],10);
            uart_ptint(USART1,"d1 ",data[1],10);
            uart_ptint(USART1,"d2 ",data[2],10);
            uart_ptint(USART1,"d3 ",data[3],10);
            uart_ptint(USART1,"d4 ",data[4],10);
            uart_ptint(USART1,"d5 ",data[5],10);
            uart_ptint(USART1,"d6 ",data[6],10);
            uart_ptint(USART1,"d7 ",data[7],10);
        }
        osDelay(50);
    }
}

void LED_Init();

int main(void) {
    setSystemClock();
    SystemCoreClockUpdate();
    HAL_Init();
    LED_Init();
    __HAL_RCC_AFIO_CLK_ENABLE();

    /** pinmux */
    PD1_PD0_TO_CAN();
    PB6_PB7_TO_UART1();
    PD5_PD6_TO_UART2();
    /** periphe */
    BSP_CAN1_INIT();
    BSP_UART1_INIT();
    BSP_UART2_INIT();

    osThreadDef(uartTask, uart_test, osPriorityNormal, 0, 128);
    osThreadDef(canTask, can_test, osPriorityNormal, 0, 128);
    osThreadDef(defaultTask, led_blinkly, osPriorityNormal, 0, 128);
    uartTaskHandle = osThreadCreate(osThread(uartTask),NULL);
    canTaskHandle = osThreadCreate(osThread(canTask),NULL);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask),NULL);

    // p_can1 = GET_CAN1_HANDLE();
    // robomodule_set_can_handle(p_can1);

    // robomodule_can_reset(0,1,10);
    // HAL_Delay(1000);
    // robomodule_can_set_mode(0,1,ROBOMODULE_OPENLOOP_MODE,10);
    // HAL_Delay(1);
    // robomodule_can_openloop_mode(0,1,2000,10);

    osKernelStart();
    while (1)
    {
        HAL_Delay(500);
    }
}

void LED_Init() {
  LED_GPIO_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}