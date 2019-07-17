#include "protocol.h"

/**
 * @brief 传输通讯帧至上位机
 * 
 * @param str 传输的字符串内容
 * @param len 通过串口传输的长度
 * @return 传输的状态
 */
bool Protocol_MsgTransimit(unsigned char *str, unsigned short len)
{
    HAL_StatusTypeDef transmit_status;

    UART_HandleTypeDef *huart = get_uartx_handle(PROTOCOL_USART_PORT);

    /*传输 */
    transmit_status = HAL_UART_Transmit(huart, str, len, 10);
    if (transmit_status == HAL_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief  计算通讯的校验数据和
 * 
 * @param str 需要计算较校验和的数据
 * @param len 数据长度
 * @return unsigned char 校验和
 */
unsigned char Protocol_CalculateChecksum(unsigned char *str, unsigned short len)
{
    unsigned char check_sum = 0x00;

    for (int i = 0; i < len; i++)
    {
        check_sum += str[i];
    }

    return check_sum;
}

/**
 * @brief 传输机器人平台系统状态给到上位机系统
 * 
 * @param fdbk_t 系统状态反馈结构体
 * @return true 传输成功
 * @return false 传输失败
 */
bool Protocol_PlatfromFdbk_ToController(PlatformFdbk_t fdbk_t)
{
    PlatformFdbk_t fdbk;
    bool transimit_status;

    fdbk.fdbk = fdbk_t.fdbk;
    
    /*计算校验和*/
    fdbk.fdbk.check_sum = Protocol_CalculateChecksum(fdbk.raw, sizeof(fdbk.fdbk) - 1);

    /*传送数据*/

    transimit_status = Protocol_MsgTransimit(fdbk.raw, sizeof(fdbk.fdbk));

    if (transimit_status == true)
    {
        return true;
    }else 
    {
        return false;
    }
}
