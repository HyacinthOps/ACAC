/*
 * pid.c
 *
 *  Created on: Aug 28, 2024
 *      Author: Huang
 */
#include "pid.h"

PID g_pid;

// 初始化 PID 控制器
void PID_Init(PID *pid, double Kp, double Ki, double Kd) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->prev_error = 0.0;
    pid->prev_prev_error = 0.0;
}

// 增量式 PID 计算
double PID_clc(PID *pid, double setpoint, double measured_value) {
    double error = setpoint - measured_value;

    // 计算增量
    double delta_output = pid->Kp * (error - pid->prev_error)
                        + pid->Ki * error
                        + pid->Kd * (error - 2 * pid->prev_error + pid->prev_prev_error);


    // 更新误差
    pid->prev_prev_error = pid->prev_error;
    pid->prev_error = error;

    return delta_output;
}
