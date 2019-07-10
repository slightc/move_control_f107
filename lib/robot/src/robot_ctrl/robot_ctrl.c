

#include "robot_ctrl.h"

/**
 * @brief 差速轮底盘运动综合
 * @param[in] chasis: 底盘相关参数
 * @param[in] *current_move_vel: 当前线速度(m/s)
 * @param[in] *current_rotate_vel: 当前旋转角速度(rad/s)
 * @param[in] target_move_velocity :线速度(m/s)
 * @param[in] rotate_velocity：旋转角速度(rad/s)
 * @param[in] move_acc: 移动加速度，这个值这个这个函数调用的周期有关系
 *                      acc = move_acc * 周期
 * @param[in] rotate_acc 旋转加速度这个值这个这个函数调用的周期有关系
 *                      acc = rotate_acc * 周期
 * @param[in] 输出rpm
 * @return
 */
bool Chasis_DiffMode_CalRpmOut(ChasisParams chasis, double *current_move_vel, double *current_rotate_vel,
                               double target_move_velocity, double target_rotate_velocity, double move_acc,
                               double rotate_acc, double *out_rpm)
{
    double out_vel[2];
    if (move_acc == 0)
    {
        /*如果加速度设置为0，可以认为速度直接过去*/
        *current_move_vel = target_move_velocity;
    }
    else
    {
        /*如果目标速度比当前速度加上加速度大，则当前速度加上加速度 */
        if (target_move_velocity - *current_move_vel > move_acc)
        {
            *current_move_vel = *current_move_vel + move_acc;
        }
        /*如果目标速度比当前速度减小加速度小，则当前速度减去加速度 */
        else if (*current_move_vel - target_move_velocity > move_acc)
        {
            *current_move_vel = *current_move_vel - move_acc;
        }
        else
        {
            *current_move_vel = target_move_velocity;
        }
    }

    if (rotate_acc == 0)
    {
        *current_rotate_vel = target_rotate_velocity;
    }
    else
    {
        if (target_rotate_velocity - *current_rotate_vel > rotate_acc)
        {
            *current_rotate_vel = *current_rotate_vel + rotate_acc;
        }
        else if (*current_rotate_vel - target_rotate_velocity > rotate_acc)
        {
            *current_rotate_vel = *current_rotate_vel - rotate_acc;
        }
        {
            *current_rotate_vel = target_rotate_velocity;
        }
    }

    /*计算输出速度m/s*/
    Chasis_DiffMode_WithTwoMotors(*current_move_vel, *current_rotate_vel, chasis.wheels_distance, out_vel);

    /*计算输出rpm*/
    out_rpm[0] = Motor_MetersPerSecondToRPM(out_vel[0], chasis.motor_gear, chasis.wheels_radius);
    out_rpm[1] = Motor_MetersPerSecondToRPM(out_vel[1], chasis.motor_gear, chasis.wheels_radius);
}

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
 * @param[in] wheels_radius: the wheels diameter(m)
 * @return  move speed (m/s)
 */
double Motor_RPMToMetersPerSecond(signed short int rmp, float gear, double wheels_radius)
{
    double velocity;

    /*将rmp 计算为m/s*/
    velocity = rmp * (Pi * wheels_radius * 2) / gear / 60.0;

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
