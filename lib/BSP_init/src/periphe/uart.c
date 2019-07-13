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