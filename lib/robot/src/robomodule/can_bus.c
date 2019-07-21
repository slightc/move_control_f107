#include "can_bus.h"

#define ROBOMODULE_RESET_ID 0x0000
#define ROBOMODULE_MODE_CHOICE_ID 0x0001
#define ROBOMODULE_OPENLOOP_ID 0x0002
#define ROBOMODULE_CURRENT_ID 0x0003
#define ROBOMODULE_VELOCITY_ID 0x0004
#define ROBOMODULE_POSITION_ID 0x0005
#define ROBOMODULE_VELOCITY_POSITION_ID 0x0006
#define ROBOMODULE_CURRENT_VELOCITY_ID 0x0007
#define ROBOMODULE_CURRENT_POSITION_ID 0x0008
#define ROBOMODULE_CURRENT_VELOCITY_POSITION_ID 0x0009
#define ROBOMODULE_CONFIG_ID 0x000A
#define ROBOMODULE_ONLINE_CHECK_ID 0x000F

#define GET_INT16_H(value) ((uint8_t)((value>>8)&0xff))
#define GET_INT16_L(value) ((uint8_t)((value)&0xff))
#define GET_DATA_SHFIT(value,bits) ((uint8_t)((value>>bits)&0xff))

/** 
 * robomodule通信中使用的can_handle指针 
 * 使用时应当判断`this_can_handle`是否为NULL
 * 为NULL时应该立即返回错误
 */
static CAN_HandleTypeDef *this_can_handle=NULL;
static int16_t robomodule_max_pwm=5000;

static void set_array8_value(uint8_t *p_data,
    uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3,
    uint8_t d4,uint8_t d5,uint8_t d6,uint8_t d7)
{
    p_data[0] = d0;
    p_data[1] = d1;
    p_data[2] = d2;
    p_data[3] = d3;
    p_data[4] = d4;
    p_data[5] = d5;
    p_data[6] = d6;
    p_data[7] = d7;
}

/**
 * @brief 设置this_can_handle指针值
 * 
 * @param that_can_handle 已经初始化好的can_handle
 */
void robomodule_set_can_handle(CAN_HandleTypeDef *that_can_handle)
{
    this_can_handle = that_can_handle;
}

/**
 * @brief 设置max_pwm值
 * 
 * @param max_pwm 最大pwm输出值(0~5000) pwm值为5000时,输出电压为电源电压
 */
void robomodule_set_max_pwm(int16_t max_pwm)
{
    /** 限制pwm数据值 */
    if(max_pwm < 0) max_pwm =-max_pwm;
    if(max_pwm > 5000) max_pwm = 5000;
    robomodule_max_pwm = max_pwm;
}

/**
 * @brief 公共发送can数据函数
 * 
 * @param fun_id ROBOMODULE_xxx_ID
 * @param can_data 发送数据指针 8byte
 * @param group 指定组(0~7)
 * @param number 指定驱动(0~15) 0为广播数据
 * @param timeout 超时时间(ms 0~0xffff)
 * @return uint8_t errCode 错误码
 */
uint8_t robomodule_can_send(uint16_t fun_id, uint8_t *can_data, 
                uint8_t group, uint8_t number, uint32_t timeout)
{
    CAN_TxHeaderTypeDef can_head;
    uint32_t can_mail=0;
    uint32_t start_time = HAL_GetTick();

    /** 判断输入参数是否正确 */
    if((this_can_handle==NULL) || (group>7) || (number>15)) return 1;
    /** 等待有可以发送的空邮箱 或者 超时 */
    while (
        HAL_CAN_GetTxMailboxesFreeLevel(this_can_handle)==0 && 
        (HAL_GetTick()-start_time)<timeout 
    ){
        //wait
    }
    /** 判断是否超时 */
    if((HAL_GetTick()-start_time)>=timeout) return 2;

    /** 设置can参数头 */
    can_head.StdId= fun_id | (group<<8) | (number<<4);
    can_head.DLC = 0x08;
    can_head.IDE=CAN_ID_STD;
    can_head.RTR=CAN_RTR_DATA;
    can_head.TransmitGlobalTime=DISABLE;
    
    /** 发送can数据 */
    HAL_CAN_AddTxMessage(this_can_handle,&can_head,can_data,&can_mail);
    return 0;
}

/**
 * @brief 复位指定的robomodule
 * 
 * @param group 指定组(0~7)
 * @param number 指定驱动(0~15) 0为广播数据
 * @param timeout 超时时间(ms 0~0xffff)
 * @return uint8_t errCode 错误码
 */
uint8_t robomodule_can_reset(uint8_t group, uint8_t number, uint32_t timeout)
{
    uint8_t can_data[8];

    /** 设置can负载数据 */
    set_array8_value(can_data,
        0x55,0x55,0x55,0x55,
        0x55,0x55,0x55,0x55);
    
    /** 发送can数据 */
    return robomodule_can_send(ROBOMODULE_RESET_ID,can_data,
        group,number,timeout);
}

/**
 * @brief 设置robomodule的工作模式
 * 
 * @param group 指定组(0~7)
 * @param number 指定驱动(0~15) 0为广播数据
 * @param mode ROBOMODULE_xxx_MODE
 * @param timeout 超时时间(ms 0~0xffff)
 * @return uint8_t errCode 错误码
 */
uint8_t robomodule_can_set_mode(uint8_t group, uint8_t number, 
                                    uint8_t mode, uint32_t timeout)
{
    uint8_t can_data[8];

    /** 设置can负载数据 */
    set_array8_value(can_data,
        mode,0x55,0x55,0x55,
        0x55,0x55,0x55,0x55);
    
    /** 发送can数据 */
    return robomodule_can_send(ROBOMODULE_MODE_CHOICE_ID,can_data,
        group,number,timeout);
}

/**
 * @brief 开环模式时设置robomodule的输出pwm
 * 
 * @param group 指定组(0~7)
 * @param number 指定驱动(0~15) 0为广播数据
 * @param pwm pwm输出值(-5000~5000) pwm值为5000时,输出电压为电源电压
 * @param timeout 超时时间(ms 0~0xffff)
 * @return uint8_t errCode 错误码
 */
uint8_t robomodule_can_openloop_mode(uint8_t group, uint8_t number, 
                                        int16_t pwm, uint32_t timeout)
{
    uint8_t can_data[8];

    /** 限制pwm数据值 */
    if(pwm > 5000) pwm = 5000;
    if(pwm <-5000) pwm =-5000;

    /** 设置can负载数据 */
    set_array8_value(can_data,
        GET_INT16_H(pwm),GET_INT16_L(pwm),
        0x55,0x55,
        0x55,0x55,0x55,0x55);
    
    /** 发送can数据 */
    return robomodule_can_send(ROBOMODULE_OPENLOOP_ID,can_data,
        group,number,timeout);
}

/**
 * @brief 电流模式时设置robomodule的最大输出pwm和电机电流
 * 
 * @param group 指定组(0~7)
 * @param number 指定驱动(0~15) 0为广播数据
 * @param current 电机电流(-32768~+32767) 单位mA
 * @param timeout 超时时间(ms 0~0xffff)
 * @return uint8_t errCode 错误码
 */
uint8_t robomodule_can_current_mode(uint8_t group, uint8_t number, 
                        int16_t current, uint32_t timeout)
{
    uint8_t can_data[8];

    /** 设置can负载数据 */
    set_array8_value(can_data,
        GET_INT16_H(robomodule_max_pwm),GET_INT16_L(robomodule_max_pwm),
        GET_INT16_H(current),GET_INT16_L(current),
        0x55,0x55,0x55,0x55);
    
    /** 发送can数据 */
    return robomodule_can_send(ROBOMODULE_CURRENT_ID,can_data,
        group,number,timeout);
}

/**
 * @brief 速度模式时设置robomodule的最大输出pwm和电机转速
 * 
 * @param group 指定组(0~7)
 * @param number 指定驱动(0~15) 0为广播数据
 * @param velocity 电机转速(-32768~+32767) 单位RPM
 * @param timeout 超时时间(ms 0~0xffff)
 * @return uint8_t errCode 错误码
 */
uint8_t robomodule_can_velocity_mode(uint8_t group, uint8_t number, 
                        int16_t velocity, uint32_t timeout)
{
    uint8_t can_data[8];

    /** 设置can负载数据 */
    set_array8_value(can_data,
        GET_INT16_H(robomodule_max_pwm) ,GET_INT16_L(robomodule_max_pwm),
        GET_INT16_H(velocity),GET_INT16_L(velocity),
        0x55,0x55,0x55,0x55);
    
    /** 发送can数据 */
    return robomodule_can_send(ROBOMODULE_VELOCITY_ID,can_data,
        group,number,timeout);
}

/**
 * @brief 位置模式时设置robomodule的最大输出pwm和当前电机位置
 * 
 * @param group 指定组(0~7)
 * @param number 指定驱动(0~15) 0为广播数据
 * @param position 电机位置(-2147483648~+2147483647) 单位qc(4倍编码器值)
 * @param timeout 超时时间(ms 0~0xffff)
 * @return uint8_t errCode 错误码
 */
uint8_t robomodule_can_position_mode(uint8_t group, uint8_t number, 
                        int32_t position, uint32_t timeout)
{
    uint8_t can_data[8];

    /** 设置can负载数据 */
    set_array8_value(can_data,
        GET_INT16_H(robomodule_max_pwm) ,GET_INT16_L(robomodule_max_pwm),
        0x55,0x55,
        GET_DATA_SHFIT(position,24),GET_DATA_SHFIT(position,16),
        GET_DATA_SHFIT(position,8), GET_DATA_SHFIT(position,0));
    
    /** 发送can数据 */
    return robomodule_can_send(ROBOMODULE_POSITION_ID,can_data,
        group,number,timeout);
}