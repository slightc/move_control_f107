

#include "robot_ctrl.h"

/**
 * @brief  两轮差速轮底盘速度解算
 * @param[in] linear_velocity：线速度(m/s)
 * @param[in] rotate_velocity：旋转角速度(rad/s)
 * @param[in] wheels_distanse： 轮距(m)
 * @param[in] out_velocity：输出(m/s)
 * @return
 */
bool Chasis_DiffMode_WithTwoMotors(double linear_velocity, double rotate_velocity,
                                   double wheels_distanse, double *out_velocity)
{
    double left_speed, right_speed;

    /*check the param wheather is null*/
    if (out_velocity == (void *)(0))
    {
        return false;
    }

    /*v = w * r*/
    left_speed = linear_velocity - rotate_velocity * (wheels_distanse / 2);
    right_speed = linear_velocity + rotate_velocity * (wheels_distanse / 2);

    out_velocity[0] = left_speed;
    out_velocity[1] = right_speed;

    return true;
}

/**
 * @brief  四轮差速轮底盘速度解算
 * @param[in] linear_velocity：线速度(m/s)
 * @param[in] rotate_velocity：旋转角速度(rad/s)
 * @param[in] wheels_distanse： 轮距(m)
 * @param[in] out_velocity：输出(m/s)
 * @return
 */
bool Chasis_DiffMode_WithFourMotors(double linear_velocity, double rotate_velocity,
                                    double rotate_radius, double *out_velocity)
{
    double left_speed, right_speed;

    /*check the param wheather is null*/
    if (out_velocity == (void *)(0))
    {
        return false;
    }

    /*v = w * r*/
    left_speed = linear_velocity - rotate_velocity / 0.656 * rotate_radius;
    right_speed = linear_velocity + rotate_velocity / 0.656 * rotate_radius;

    out_velocity[0] = left_speed;
    out_velocity[1] = right_speed;
    out_velocity[2] = right_speed;
    out_velocity[3] = left_speed;

    return true;
}

/**
  * @brief
  * @param[in]
  * @return
  */
bool Chasis_Ackermann_WithFourMotors(double linear_velocity, double angular_velocity,
                                     double wheels_distanse, double *out_velocity)
{
    if (out_velocity == (void *)(0))
    {
        return false;
    }

    return true;
}

/**
 * @brief  麦克纳木轮四轮驱动解算
 * @param[in] linear_velocity ：移动线速度(m/s)
 * @param[in] move_angle ：移动角度(rad)
 * @param[in] rotate_velocity ：旋转角速度(rad/s)
 * @param[in] axis_distanse ：轴距(m)
 * @param[in] wheels_distanse ：轮距(m)
 * @param[in] out_velocity ：输出(m/s)
 * @return
 */
bool Chasis_Mecanum_WithFourMotors(double linear_velocity, double move_angle, double rotate_velocity,
                                   double axis_distanse, double wheels_distanse, double *out_velocity)
{
    double x_axis_velocity, y_axis_velocity;

    if (out_velocity == (void *)(0))
    {
        return false;
    }

    /*速度分解，求出x 和 y方向的分速度*/
    x_axis_velocity = linear_velocity * cos(move_angle);
    y_axis_velocity = linear_velocity * sin(move_angle);

    /*速度解算，求解四个轮子的速度*/
    out_velocity[0] = y_axis_velocity - x_axis_velocity + rotate_velocity * (axis_distanse + wheels_distanse) / 2;
    out_velocity[1] = y_axis_velocity + x_axis_velocity - rotate_velocity * (axis_distanse + wheels_distanse) / 2;
    out_velocity[2] = y_axis_velocity - x_axis_velocity - rotate_velocity * (axis_distanse + wheels_distanse) / 2;
    out_velocity[3] = y_axis_velocity + x_axis_velocity + rotate_velocity * (axis_distanse + wheels_distanse) / 2;

    return true;
}

/**
 * @brief  convert m/s to rpm
 * @param[in] m_move_velocity: move speed (m/s)
 * @param[in] gear : the gear
 * @param[in] diameter: the wheels diameter(m)
 * @return  rpm
 */
signed short int Motor_MetersPerSecondToRPM(double m_move_velocity, float gear, double radius)
{
    signed short int out_rpm;

    /*计算该速度的输出rpm*/
    out_rpm = m_move_velocity * 60.0 * gear / (Pi * radius * 2);

    return out_rpm;
}

/**
 * @brief  convert rpm to m/s
 * @param[in] rpm
 * @param[in] gear : the gear
 * @param[in] diameter: the wheels diameter(m)
 * @return  move speed (m/s)
 */
double Motor_RPMToMetersPerSecond(signed short int rmp, float gear, double radius)
{
    double velocity;

    /*将rmp 计算为m/s*/
    velocity = rmp * (Pi * radius * 2) / gear / 60.0;

    return velocity;
}

/**
 * @brief  transfrom rad to degree
 * @param[in] rad
 * @return  degree
 */
double ChasisCommon_RadToDegree(double rad)
{
    return 180.0 * rad / Pi;
}

/**
 * @brief  transfrom degree to rad
 * @param[in] degree
 * @return  rad
 */
double ChasisCommon_DegreeToRad(double degree)
{
    return degree * Pi / 180.0;
}


