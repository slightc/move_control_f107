#include "main.h"
#include "system/system_init.h"
#include "pinmux/pin_to_can.h"
#include "periphe/can.h"
#include "robomodule/can_bus.h"
#include "cmsis_os.h"

osThreadId defaultTaskHandle;

osThreadDef(defaultTask, NULL, osPriorityNormal, 0, 128);

void LED_Init();

int main(void) {
    CAN_HandleTypeDef *p_can1;

    setSystemClock();
    SystemCoreClockUpdate();
    HAL_Init();
    LED_Init();
    __HAL_RCC_AFIO_CLK_ENABLE();
    PD1_PD0_TO_CAN();
    CAN1_INIT();

    p_can1 = CAN1_HANDLE();
    robomodule_set_can_handle(p_can1);

    robomodule_can_reset(0,1,10);
    HAL_Delay(1000);
    robomodule_can_set_mode(0,1,ROBOMODULE_OPENLOOP_MODE,10);
    HAL_Delay(1);
    robomodule_can_openloop_mode(0,1,2000,10);

    
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
  // AFIO_MAPR_CAN_REMAP
}

void SysTick_Handler(void) {
  HAL_IncTick();
}