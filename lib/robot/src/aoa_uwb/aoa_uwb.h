#ifndef __AOA_UWB_H__
#define __AOA_UWB_H__

#ifdef __cplusplus
extern "C"
{
#endif

// #define GetUsartRece(unsigned char *c) 

#include "string.h"
#define AOA_SOF 0x59
#define AOA_EOF 0x47
#define AOA_REPORT 0x63
#define AOA_REQUEST 0x64

    /**
     * @brief sof:侦起始符
     * @brief length ：数据长度
     * @brief type : 数据类型
     */
    typedef struct
    {
        unsigned char sof;
        unsigned char length;
        unsigned char type;
        unsigned char frame_index;
        char rx_rssi_first;
        char rx_rssi_all;
        char battery;
        unsigned char key;
        unsigned short tagID;
        unsigned short distance;
        short angle;
        unsigned char status;
        unsigned char quality;
        unsigned short checksum;
        unsigned char eof;
    } AOA_Report_t;

    /**
     * @brief AOA 请求帧
     * 
     */
    typedef struct
    {
        unsigned char sof;
        unsigned char length;
        unsigned char type;
        unsigned char frame_index;
        unsigned char output_dist;
        unsigned char status;
        unsigned char battery;
        unsigned char speed;
        unsigned short checksum;
        unsigned char eof;
    } AOA_Request_t;

    /**
     * @brief distance : 移动标把到固定标签的距离
     * @brief angle : 角度信息
     */
    typedef struct
    {
        unsigned short distance;
        short angle;
    } AOA_FollowTag_t;

    unsigned short Get_Crc16(unsigned char *pucFrame, unsigned char usLen);

#ifdef __cplusplus
}
#endif

#endif
