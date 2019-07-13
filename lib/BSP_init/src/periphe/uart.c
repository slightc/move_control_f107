#include "uart.h"

static UART_HandleTypeDef huart[5];

UART_HandleTypeDef* get_uartx_handle(USART_TypeDef *UARTx)
{
    int8_t index=-1;
    switch ((uint32_t)(UARTx))
    {
        case (uint32_t)(UART5) : index++;
        case (uint32_t)(UART4) : index++;
        case (uint32_t)(USART3): index++;
        case (uint32_t)(USART2): index++;
        case (uint32_t)(USART1): index++;
            break;
        default:
            break;
    }
    if(index==-1){
        return NULL;
    }else{
        return (huart+index);
    }
}

uint8_t uart_init(USART_TypeDef *UARTx, uint32_t baud_rate)
{
    UART_HandleTypeDef* p_huart = get_uartx_handle(UARTx);

    p_huart->Instance        = UARTx;
    p_huart->Init.BaudRate   = baud_rate;
    p_huart->Init.WordLength = UART_WORDLENGTH_8B;
    p_huart->Init.StopBits   = UART_STOPBITS_1;
    p_huart->Init.Parity     = UART_PARITY_NONE;
    p_huart->Init.Mode       = UART_MODE_TX_RX;

    return HAL_UART_Init(p_huart);
}

/**
 * @brief 读取串口
 * 
 * @param phuart 串口操作句柄
 * @param buffer 数据读出的地址指针
 * @param len 读数据的长度
 * @param timeout 读取超时 单位ms
 * @return uint8_t 读取到的数据长度
 */
uint8_t uart_read(UART_HandleTypeDef *phuart, uint8_t *buffer, uint8_t len, uint32_t timeout)
{

}