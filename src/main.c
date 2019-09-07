#include "main.h"
#include "stm32f1xx_it.h"
#include "system/system_init.h"
//=============================
#include "pinmux/pin_to_can.h"
#include "pinmux/pin_to_uart.h"
#include "pinmux/pin_to_iic.h"
//=============================
#include "periphe/can.h"
#include "periphe/uart.h"
#include "periphe/iic.h"
//=============================
#include "robomodule/can_bus.h"
#include "cmsis_os.h"
#include "aoa_uwb/aoa_uwb.h"
#include "robot_protocal/protocol.h"

#define CHG_INT16_HL(n) ( ((n&0xff)<<8) |((n>>8)&0xff) )

#define LEFT_WHEEL  0
#define RIGHT_WHEEL 1
int16_t CURRENT_WHEEL[2] = {0};
int8_t run_mode = 0;

/** 左负右正 -300~300 超出范围为朝某方向丢失 */
int16_t magnetic_strip_position = 0;
int8_t magnetic_strip_online = 0;

osThreadId startTaskHandle;
osThreadId defaultTaskHandle;
osThreadId uartTaskHandle;
osThreadId canTaskHandle;
int16_t aoa_distance=0;
int16_t aoa_angle=0;


void LED_Init();

void set_wheel_speed(uint8_t wheel,int16_t speed);

void set_wheel_speed(uint8_t wheel,int16_t speed){
    // if(speed == CURRENT_WHEEL[wheel]) return;
    robomodule_can_velocity_mode(0,wheel+1,-speed,5);
}


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
    AOA_Report_t *p=NULL;
    int16_t distance=0;
  
    (void) argument;
    p_huart1 = GET_UART1_HANDLE();
	for (;;)
    {   
        p=wait_aoa_report_packet(100);
        // HAL_UART_Transmit(p_huart1,send_str,strlen(send_str),5);
        uart_printf(USART1,"wait_aoa_report_packet\n");
        if(p==NULL)
        {
            uart_printf(USART1,"timeout\n");
        }else{
            uart_printf(USART1,"have packet\n");
            aoa_distance = (int16_t)CHG_INT16_HL(p->distance);
            aoa_angle = aoa_angle*0.5 + 0.5* (int16_t)CHG_INT16_HL(p->angle);
            distance = 0.98*distance + 0.02*(int16_t)CHG_INT16_HL(p->distance);
            uart_printf(USART1,"d: %d a: %d \n",distance,(int16_t)CHG_INT16_HL(p->angle));
        }
        osDelay(50);
        // for(uint8_t i=0;i<100;i++){
        //     AOA_Tag_Handler();
        // }
        // p = AOA_GetMsg();
        // uart_read(USART1,send_str,10,10000);
        // HAL_UART_Transmit(p_huart1,send_str,10,5);
        // osDelay(500);
        // uart_print(USART1,"d:",CHG_INT16_HL(p->distance),10);
        // uart_print(USART1,"a:",CHG_INT16_HL(p->angle),10);
    }
}

int16_t calc_magnetic_strip_position(int8_t *mag_data,int16_t old_position){
    int16_t max_value,max_index;
    int16_t new_position;
    int8_t i1,i2,i3;

    for(uint8_t i=0;i<8;i++){
        mag_data[i] = -mag_data[i];
    }
    max_index = 0;
    max_value = mag_data[max_index];
    for(uint8_t i=1;i<8;i++){
        if(mag_data[i]>max_value){
            max_value = mag_data[i];
            max_index = i;
        }
    }
    // 15 30 60
    if(max_value<15){
        if(old_position<0){
            new_position = -320;
        }else{
            new_position = 320;
        }
    }else if(max_value<30 && (max_index==0 || max_index==7)){
        if(max_index==0){
            new_position = 300;
        }else{
            new_position = -300;
        }
    }else{
        if(max_index==0){
            new_position = 300;
        }else if(max_index==7){
           new_position = -300;
        }else{
            i1 = max_index-1;
            i2 = max_index;
            i3 = max_index+1;
            if(mag_data[i1]>mag_data[i3]){
                new_position = (3.5-max_index)*100.0 + 100.0*(mag_data[i1] - mag_data[i3])/(mag_data[i1] + mag_data[i2] - 2*mag_data[i3]);
            }else{
                new_position = (3.5-max_index)*100.0 - 100.0*(mag_data[i3] - mag_data[i1])/(mag_data[i3] + mag_data[i2] - 2*mag_data[i1]);
            }
        }
        
    }
    return new_position;
}

void magnetic_strip_monitor(void const *argument){
    CAN_HandleTypeDef *p_can1;
    CAN_RxHeaderTypeDef can1_rx_header;
    int8_t data[8],len;
    int8_t lost_times = 0;

    (void) argument;

    p_can1 = GET_CAN1_HANDLE();
	for (;;)
    {   
        // uart_print(USART1,"can_test",0,10);
        lost_times ++;
        len = HAL_CAN_GetRxFifoFillLevel(p_can1,CAN_FILTER_FIFO1);
        if(len!=0){
            HAL_CAN_GetRxMessage(p_can1,CAN_FILTER_FIFO1,&can1_rx_header,data);
            uart_printf(USART1,"id:%d DLC:%d",can1_rx_header.StdId,can1_rx_header.DLC);
            uart_printf(USART1,"d0:%d d1:%d d2:%d d3:%d ",data[0],data[1],data[2],data[3]);
            uart_printf(USART1,"d4:%d d5:%d d6:%d d7:%d\n",data[4],data[5],data[6],data[7]);
            lost_times = 0;
            magnetic_strip_online = 1;
            magnetic_strip_position = calc_magnetic_strip_position(data,magnetic_strip_position);
            uart_printf(USART1,"position:%d\n",magnetic_strip_position);

        }
        if(lost_times>10){
            magnetic_strip_online = 0;
        }
        osDelay(50);
    }
}

void mode_running(void const *argument){

    (void) argument;
	for (;;)
    {   
        // switch(run_mode){
        //     default: 
        //         set_wheel_speed(LEFT_WHEEL,0);
        //         set_wheel_speed(RIGHT_WHEEL,0);
        // }
        if(0){
            float r = 0.6;
            int16_t f = 200 - 0.6*abs(magnetic_strip_position);
            if(f<0) f=0;
            set_wheel_speed(LEFT_WHEEL,f+r*magnetic_strip_position);
            set_wheel_speed(RIGHT_WHEEL,f-r*magnetic_strip_position);
            osDelay(100);
        }
        if(0){
            float r = 0.4;
            int16_t f = 0;
            set_wheel_speed(LEFT_WHEEL,f+r*aoa_angle);
            set_wheel_speed(RIGHT_WHEEL,f-r*aoa_angle);
            osDelay(100);
        }
        osDelay(10);
    }
}

void iic_read(void const *argument)
{
    (void) argument;
    I2C_HandleTypeDef *p_iic = GET_IIC1_HANDLE();
    uint8_t data[8] = { 0 };
    for (;;)
    {
        HAL_I2C_Master_Receive(p_iic,6,data,8,20);
        osDelay(100);
    }
}

void robot_ctrl(void const *argument)
{
    (void) argument;
    uint8_t data[20] = { 0 };
    uint8_t *res = NULL,timeout_count = 0;
    for (;;)
    {
        res = wait_protocol_package(data,200);
        if(res!=NULL) {
            timeout_count = 0;
            if(data[1] == 0x01){
                int8_t speed,rotate;
                int16_t r_w,l_w,rk=20;
                speed = data[2];
                rotate = data[3];
                r_w = - speed*rk - rotate*rk;
                l_w = - speed*rk + rotate*rk;
                robomodule_can_velocity_mode(0,1,r_w,10);
                robomodule_can_velocity_mode(0,2,l_w,10);
                // send_protocol_package(data,10);
            }
        }else{
            timeout_count ++ ;
            if(timeout_count>3){
                robomodule_can_velocity_mode(0,1,0,10);
                robomodule_can_velocity_mode(0,2,0,10);
            }
        }
        // osDelay(100);
    }
}

osThreadDef(uartTask, uart_test, osPriorityNormal, 0, 128);
osThreadDef(magneticTask, magnetic_strip_monitor, osPriorityNormal, 0, 128);
osThreadDef(defaultTask, led_blinkly, osPriorityNormal, 0, 128);
osThreadDef(modeRunningTask, mode_running, osPriorityNormal, 0, 128);
osThreadDef(iicTask, robot_ctrl, osPriorityNormal, 0, 256);

void start_task(void const *argument){
    (void) argument;
    for(;;){
        LED_Init();
        /** pinmux */
        PD1_PD0_TO_CAN();
        PB6_PB7_TO_UART1();
        PD5_PD6_TO_UART2();
        PB8_PB9_TO_IIC1();
        /** periphe */
        BSP_CAN1_INIT();
        BSP_UART1_INIT();
        BSP_UART2_INIT();
        BSP_IIC1_INIT();
        /** app */
        robomodule_set_can_handle(GET_CAN1_HANDLE());
        robomodule_set_max_pwm(5000);
        robomodule_can_reset(0,0,10);
        osDelay(500);
        robomodule_can_set_mode(0,0,ROBOMODULE_VELOCITY_MODE,10);
        // robomodule_can_velocity_mode(0,1,100,10);
        // robomodule_can_velocity_mode(0,2,-100,10);

        osThreadCreate(osThread(defaultTask),NULL);
        osThreadCreate(osThread(uartTask),NULL);
        osThreadCreate(osThread(magneticTask),NULL);
        osThreadCreate(osThread(modeRunningTask),NULL);
        osThreadCreate(osThread(iicTask),NULL);
        osThreadSuspend(startTaskHandle);
    }
}

int main(void) {

    setSystemClock();
    SystemCoreClockUpdate();
    HAL_DeInit();
    HAL_Init();
    __HAL_RCC_AFIO_CLK_ENABLE();

    osThreadDef(startTask, start_task, osPriorityNormal, 0, 128);
    startTaskHandle = osThreadCreate(osThread(startTask),NULL);

    osKernelStart();
    while (1) {}
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