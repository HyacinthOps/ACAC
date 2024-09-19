/*
 * pid.h
 *
 *  Created on: Aug 28, 2024
 *      Author: Huang
 */

#ifndef INC_PID_H_
#define INC_PID_H_



typedef struct {
    double Kp;         // 比例系数
    double Ki;         // 积分系数
    double Kd;         // 微分系数
    double prev_error; // 上一次误差
    double prev_prev_error; // 上上次误差
}PID;

extern PID g_pid;

void PID_Init(PID *pid, double Kp, double Ki, double Kd);
double PID_clc(PID *pid, double setpoint, double measured_value);



#endif /* INC_PID_H_ */
