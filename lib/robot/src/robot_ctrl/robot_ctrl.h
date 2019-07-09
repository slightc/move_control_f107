
#ifndef __MODULE_CHASIS_H__
#define __MODULE_CHASIS_H__

#include <stdbool.h>
#include <math.h>

#define Pi 3.1415923f

#ifdef __cplusplus
extern "C"
{
#endif

    /**
 * @name 定义不同类型机器人底盘的基础参数
 * @param max_move_velocity ：maxmin move velocity
 * @param max_angular_velocity ：maxin angular velocity
 * @param max_rotate_velocity ：maxin_angular velocity
 * @param wheels_distance ：the wheels distance of robot chasis
 * @param motor_gear : the gear between the motor and wheels
 * @param axis_distance : the axis distance
 * @{ */
    typedef struct _ChasisParams
    {
        double max_move_velocity;
        double max_angular_velocity;
        double max_rotate_velocity;
        double wheels_distance;
        double wheels_radius;
        double motor_gear;
        double axis_distance;
        double rotate_radius;
    } ChasisParams;

    /**  @} */

    bool Chasis_DiffMode_WithTwoMotors(double linear_velocity, double rotate_velocity,
                                       double wheels_distanse, double *out_velocity);

    bool Chasis_DiffMode_WithFourMotors(double linear_velocity, double rotate_velocity,
                                        double rotate_radius, double *out_velocity);

    bool Chasis_Ackermann_WithFourMotors(double linear_velocity, double angular_velocity,
                                         double wheels_distanse, double *out_velocity);

    bool Chasis_Mecanum_WithFourMotors(double linear_velocity, double move_angle, double rotate_velocity,
                                       double axis_distanse, double wheels_distanse, double *out_velocity);

    signed short int Motor_MetersPerSecondToRPM( double m_move_velocity, float gear, double radius );
    double Motor_RPMToMetersPerSecond( signed short int rmp, float gear, double radius );
    double ChasisCommon_RadToDegree(double rad);
    double ChasisCommon_DegreeToRad(double degree);

#ifdef __cplusplus
}
#endif

#endif
