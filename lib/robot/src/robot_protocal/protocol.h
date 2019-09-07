#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include <stdint.h>
#include "protocal_types.h"

#define PROTOCOL_USART_PORT USART1

#ifdef __cplusplus
extern "C"
{
#endif
    bool Protocol_MsgTransimit(unsigned char *str, unsigned short len);
    unsigned char Protocol_CalculateChecksum(unsigned char *str, unsigned short len);
    bool Protocol_PlatfromFdbk_ToController(PlatformFdbk_t fdbk_t);
    uint8_t *send_protocol_package(uint8_t *in, uint32_t timeout);
    uint8_t *wait_protocol_package(uint8_t *out, uint32_t timeout);
#ifdef __cplusplus
}
#endif

#endif
