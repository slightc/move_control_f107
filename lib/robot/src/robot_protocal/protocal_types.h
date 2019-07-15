#ifndef __PROTOCOL_TYPES_H__
#define __PROTOCOL_TYPES_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /**
 * @brief 
 * 
 */
    typedef enum
    {
        /* data */
        eRemoterCtrlMode = 0x00,
        eAutoFollowCtrlMode = 0x01,
        eAutoDectCtrlMode = 0x02
    } CtrlMode_t;

    /**
 * @brief 
 * 
 */
    typedef union protocal_types {
        struct fdbk
        {
            unsigned short sof;
            char data_lens;
            unsigned short voltage_mv;
            CtrlMode_t ctrl_mode;
            short rotate_vel_mm_s;
            short move_vel_mmrad_s;
            bool is_aoa_uwb_online;
        };
        unsigned char *raw;
    } PlatformFdbk_t;

    /**
     * @brief 
     * 
     */
    typedef union {
        struct cmd
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