/*
 * collect.h
 *
 *  Created on: Sep 17, 2024
 *      Author: Huang
 */

#ifndef INC_IM1281B_H_
#define INC_IM1281B_H_


#include "main.h"
#include "string.h"
#include "oled.h"
#include "usart.h"



#define MODBUS_FRAME_DEVICE_BYTE 0x01
#define FRAME_RECEIVE_LENGTH 37
#define MODBUS_FRAME_WRITE_BYTE 0x10
#define MODBUS_FRAME_READ_BYTE 0x03


typedef struct {
    float U;  // 电压
    float I;  // 电流
    float P;  // 功率
    float PF;  // 功率因数
    float F;  // 频率
} AC_PARAM;



extern uint8_t g_get_ac_param_flag;
extern uint8_t g_uart_rx_buffer[FRAME_RECEIVE_LENGTH];
extern uint8_t g_uart_rx_index;
extern uint8_t g_uart_frame_received;
extern uint8_t g_uart_rx_byte;//MODBUS一个一个字节的接收
extern AC_PARAM g_AC_IN;

void Show_AC_Param(void);

#endif /* INC_IM1281B_H_ */
