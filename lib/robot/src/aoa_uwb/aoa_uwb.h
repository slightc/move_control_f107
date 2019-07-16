#ifndef __AOA_UWB_H__
#define __AOA_UWB_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "string.h"
#include "periphe/uart.h" 

#define GET_BUFFER_LEN() get_uart_rx_count(USART2)
#define AOA_SOF        (0x59)
#define AOA_EOF        (0x47)
#define AOA_REPORT     (0x63)
#define AOA_REQUEST    (0x64)
#define AOA_REPORT_LEN (19)

/**
 * @brief sof:侦起始符
 * @brief length ：数据长度
 * @brief type : 数据类型
 */
typedef struct
{
    //unsigned char sof;
    //unsigned char length;
    //unsigned char type;
    uint8_t  frame_index;
    int8_t   rx_rssi_first;
    int8_t   rx_rssi_all;
    int8_t   battery;
    uint8_t  key;
    uint16_t tagID;
    uint16_t distance;
    int16_t  angle;
    uint8_t  status;
    uint8_t  quality;
    uint16_t checksum;
    uint8_t  eof;
} AOA_Report_t;

/**
 * @brief 
 * 
 */
typedef struct
{
    uint8_t      sof;
    uint8_t      length;
    uint8_t      type;
    AOA_Report_t report;
} AOA_Tag_t;

/**
 * @brief AOA 请求帧
 * 
 */
typedef struct
{
    uint8_t  sof;
    uint8_t  length;
    uint8_t  type;
    uint8_t  frame_index;
    uint8_t  output_dist;
    uint8_t  status;
    uint8_t  battery;
    uint8_t  speed;
    uint16_t checksum;
    uint8_t  eof;
} AOA_Request_t;

/**
 * @brief distance : 移动标把到固定标签的距离
 * @brief angle : 角度信息
 */
typedef struct
{
    uint16_t distance;
    int16_t  angle;
} AOA_FollowTag_t;

unsigned short Get_Crc16(unsigned char *pucFrame, unsigned char usLen);
AOA_Report_t *AOA_GetMsg(void);
void AOA_Tag_Handler(void);
AOA_Report_t *wait_aoa_report_packet(uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif
