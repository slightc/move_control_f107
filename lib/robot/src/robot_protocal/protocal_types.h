#ifndef __PROTOCOL_TYPES_H__
#define __PROTOCOL_TYPES_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /**
 * @brief 控制模式枚举
 * 
 */
    typedef enum
    {
        /* data */
        eRemoterCtrlMode = 0x00,
        eAutoFollowCtrlMode = 0x01,
        eAutoDectCtrlMode = 0x02
    } CtrlMode_t;

typedef struct
{
    unsigned short sof;
    char data_lens;
    unsigned short voltage_mv;
    CtrlMode_t ctrl_mode;
    short rotate_vel_mm_s;
    short move_vel_mmrad_s;
    bool is_aoa_uwb_online;
    unsigned check_sum;
} protocal_fb_t;
    /**
 * @brief  底盘反馈数据结构体
 * 
 */
    typedef union protocal_types {
        protocal_fb_t fdbk;
        unsigned char *raw;
    } PlatformFdbk_t;

    /**
     * @brief 上位机下发的控制结构体
     */
    typedef union {
        struct
        {
            CtrlMode_t ctrl_mode;
            short rotate_vel_mm_s;
            short move_vel_mmrad_s;
        };
        unsigned char *row;
    } PlatformCtrlCmd_t;

#ifdef __cplusplus
}
#endif

#endif