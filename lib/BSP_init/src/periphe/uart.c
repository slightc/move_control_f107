#include "uart.h"

#define RX_BUFFER_SIZE (50)

static UART_HandleTypeDef huart[5];
static uint8_t uart_rx_buffer[5][RX_BUFFER_SIZE];
static uint8_t uart_rx_buffer_position[5]={0};

int8_t get_uartx_index(USART_TypeDef *UARTx)
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
    return index;
}

UART_HandleTypeDef* get_uartx_handle(USART_TypeDef *UARTx)
{
    int8_t index = get_uartx_index(UARTx);
    if(index==-1){
        return NULL;
    }else{
        return (huart+index);
    }
}

uint8_t *get_uartx_rx_buffer(USART_TypeDef *UARTx)
{
    int8_t index = get_uartx_index(UARTx);
    if(index==-1){
        return NULL;
    }else{
        return (uart_rx_buffer[index]);
    }
}

uint8_t *get_uartx_rx_buffer_position(USART_TypeDef *UARTx)
{
    int8_t index = get_uartx_index(UARTx);
    if(index==-1){
        return NULL;
    }else{
        return (uart_rx_buffer_position+index);
    }
}

uint8_t uart_start_recive(USART_TypeDef *UARTx)
{
    UART_HandleTypeDef *huart = get_uartx_handle(UARTx);
    if(UARTx==USART1){
        HAL_NVIC_SetPriority(USART1_IRQn, 4, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
    HAL_UART_Receive_IT(huart,get_uartx_rx_buffer(huart->Instance),RX_BUFFER_SIZE);
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

    HAL_UART_Init(p_huart);
    uart_start_recive(UARTx);
    return 1;
}

/**
 * @brief 读取串口
 * 
 * @param UARTx 串口x
 * @param buffer 数据读出的地址指针
 * @param len 读数据的长度
 * @param timeout 读取超时 单位ms
 * @return uint8_t 读取到的数据长度
 */
uint8_t uart_read(USART_TypeDef *UARTx, uint8_t *buffer, uint8_t len, uint32_t timeout)
{
    UART_HandleTypeDef *huart = get_uartx_handle(USART1);
    uint8_t *rx_buffer = get_uartx_rx_buffer(huart->Instance);
    uint8_t *p_pos = get_uartx_rx_buffer_position(UARTx);
    uint8_t data_len;
    uint8_t now_position;
    uint32_t start_time = HAL_GetTick();
    
    do{
        now_position = huart->RxXferSize - huart->RxXferCount;
        if(now_position<(*p_pos)){
            data_len = huart->RxXferSize + now_position - (*p_pos);
        }else{
            data_len = now_position - (*p_pos);
        }
    }while ((data_len<len) && ( HAL_GetTick()-start_time ) < timeout);

    if(data_len>len) data_len =len;
    for(uint8_t i=0;i<data_len;i++){
        *(buffer+i) = rx_buffer[(*p_pos)%huart->RxXferSize];
        *p_pos = *p_pos + 1;
    }
    return data_len;
}


void USART1_IRQHandler(void)
{
    UART_HandleTypeDef *huart = get_uartx_handle(USART1);
    uint8_t *p_pos = get_uartx_rx_buffer_position(USART1);
    uint32_t isrflags   = READ_REG(huart->Instance->SR);
    uint32_t cr1its     = READ_REG(huart->Instance->CR1);
    uint32_t cr3its     = READ_REG(huart->Instance->CR3);
    uint8_t now_position;

    HAL_UART_IRQHandler(huart);
    if(((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET)){
        now_position = huart->RxXferSize - huart->RxXferCount;
        if(now_position==(*p_pos)){
            *p_pos = (now_position + 1) % huart->RxXferSize;
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    HAL_UART_Receive_IT(huart,get_uartx_rx_buffer(huart->Instance),huart->RxXferSize);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    uint8_t msg[] = "error";
    HAL_UART_Transmit(huart,msg,5,100);
}



