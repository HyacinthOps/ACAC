/*
 * vofa.h
 *
 *  Created on: Aug 23, 2024
 *      Author: GAOTIANHAO
 */

#ifndef INC_VOFA_H_
#define INC_VOFA_H_

#include "usart.h"
#include "stdlib.h"
#include <math.h>
#include <string.h>
#include "pid.h"
#include "stdio.h"

//================VOFA HEX库=====================
typedef struct {
	uint8_t u8;
	uint16_t u16;
	uint32_t u32;
}VOFA;
//================VOFA HEX库=====================

//================VOFA STR库=====================
#define VOFA_FRAME_STR_FRONT '='
#define VOFA_FRAME_STR_NUM '.'
#define VOFA_FRAME_STR_LAST '!'
#define ASCII_TRANS 48
#define VOFA_FRAME_RECEIVE_MAX_LENGTH 40
#define VOFA_SET_OK 1
#define VOFA_SET_NO 0

extern uint8_t g_vofa_rx_buffer[VOFA_FRAME_RECEIVE_MAX_LENGTH];
extern uint8_t g_vofa_frame_received;
extern uint8_t g_vofa_frame_size;
extern uint8_t g_vofa_frame_byte;
extern uint8_t g_vofa_rx_index;


void Vofa_Test();


//================VOFA STR库=====================
#endif /* INC_VOFA_H_ */
