/**
 * @file pid.c
 * @brief pid work function   
 * @version 1.0
 * @updata_note
 */

#include "pid.h"

float GAP_Angle = 0.03, GAP_Position = 0.0;

/**
 * @brief    
 * @param[in] 
 * @return    
 */
double Angle_Calculator(PID_Data_t *pid_data, double error_value)
{
    if (abs(error_value) < GAP_Angle)
    {
        error_value = 0.0;
    }

    /*保存上一次的误差值*/
    pid_data->err_Last = pid_data->err;

    /*计算当前误差*/
    pid_data->err = error_value;

    /*将误差进行积分*/
    pid_data->err_I += pid_data->err;

    /*计算微分偏差*/
    pid_data->err_D = pid_data->err - pid_data->err_Last;

    /*积分限幅*/
    if (pid_data->err_I > pid_data->err_IMAX)
        pid_data->err_I = pid_data->err_IMAX;
    if (pid_data->err_I < -pid_data->err_IMAX)
        pid_data->err_I = -pid_data->err_IMAX;

    /*输出PID计算值*/
    pid_data->Out = pid_data->err * pid_data->P + pid_data->err_I * pid_data->I + pid_data->err_D * pid_data->D;

    if (pid_data->Out > pid_data->Out_MAX)
    {
        pid_data->Out = pid_data->Out_MAX;
    }

    if (pid_data->Out < -pid_data->Out_MAX)
    {
        pid_data->Out = -pid_data->Out_MAX;
    }

    return pid_data->Out;
}

double Velocity_Calculate(PID_Data_t *pid_data, double error_value)
{
    if (abs(error_value) < GAP_Position)
    {
        error_value = 0.0;
    }
    /*保存上一次的误差值*/
    pid_data->err_Last = pid_data->err;

    /*计算当前误差*/
    pid_data->err = error_value;

    /*将误差进行积分*/
    pid_data->err_I += pid_data->err;

    /*计算微分偏差*/
    pid_data->err_D = pid_data->err - pid_data->err_Last;

    /*积分限幅*/
    if (pid_data->err_I > pid_data->err_IMAX)
        pid_data->err_I = pid_data->err_IMAX;
    if (pid_data->err_I < -pid_data->err_IMAX)
        pid_data->err_I = -pid_data->err_IMAX;

    /*输出PID计算值*/
    pid_data->Out = pid_data->err * pid_data->P + pid_data->err_I * pid_data->I + pid_data->err_D * pid_data->D;

    if (pid_data->Out > pid_data->Out_MAX)
    {
        pid_data->Out = pid_data->Out_MAX;
    }

    if (pid_data->Out < -pid_data->Out_MAX)
    {
        pid_data->Out = -pid_data->Out_MAX;
    }

    return pid_data->Out;
}
