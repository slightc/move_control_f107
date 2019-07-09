/**
 * @file pid.h
 * @brief  pid head file     
 * @version 1.1
 * @updata_note 
 *         v1.1 优化代码，改用指针结构体传值，优化结构体命名
 * @todo
 *   1,在定义pid结构体时，只需要给前五个变量赋初始值
 */

#ifndef __PID_H__
#define __PID_H__

#define GAP 0.0

#define abs(x) ((x) > 0 ? (x) : (-(x)))

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        float P;
        float I;
        float D;
        float err_IMAX;
        float Out_MAX;
        float err;
        float err_Last;
        float err_I;
        float err_D;
        float Out;
    } PID_Data_t;

    double Angle_Calculator(PID_Data_t *pid_data, double error_value);
    double Velocity_Calculate(PID_Data_t *pid_data, double error_value);

#ifdef __cplusplus
}
#endif

#endif
