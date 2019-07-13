#include "main.h"
#include "system/system_init.h"
#include "pinmux/pin_to_can.h"
#include "pinmux/pin_to_uart.h"
#include "periphe/can.h"
#include "periphe/uart.h"
#include "robomodule/can_bus.h"
#include "cmsis_os.h"
#include "stm32f1xx_it.h"

osThreadId defaultTaskHandle;

void led_blinkly(void const *argument){
    UART_HandleTypeDef *p_huart1;
    uint8_t send_str[] = "qweasdzxc\n";
  
    (void) argument;
    p_huart1 = GET_UART1_HANDLE();
	for (;;)
    {   
        HAL_UART_Transmit(p_huart1,send_str,10,5);
        HAL_GPIO_TogglePin(LED_GPIO_PORT,LED_PIN);
        osDelay(1000);
    }
}


void LED_Init();

int main(void) {
    CAN_HandleTypeDef *p_can1;
    

    setSystemClock();
    SystemCoreClockUpdate();
    HAL_Init();
    LED_Init();
    __HAL_RCC_AFIO_CLK_ENABLE();

    /** pinmux */
    PD1_PD0_TO_CAN();
    PB6_PB7_TO_UART1();
    /** periphe */
    BSP_CAN1_INIT();
    BSP_UART1_INIT();

    osThreadDef(defaultTask, led_blinkly, osPriorityNormal, 0, 128);
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