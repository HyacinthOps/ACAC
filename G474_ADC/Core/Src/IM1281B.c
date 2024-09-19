/*
 * collect.cpp
 *
 *  Created on: Sep 17, 2024
 *      Author: Huang
 */
#include <IM1281B.h>

uint8_t g_uart_rx_buffer[FRAME_RECEIVE_LENGTH];
uint8_t g_uart_rx_index = 0;
uint8_t g_uart_frame_received = 0;
uint8_t g_uart_rx_byte;//MODBUS一个一个字节的接收
uint8_t g_get_ac_param_flag=0;


/*电压互感器部分*/

AC_PARAM g_AC_IN={
		0,
		0,
		0,
		0,
		0
};//输入交流电结构体实例化
AC_PARAM AC_OUT={
		0,
		0,
		0,
		0,
		0
};//输出交流电结构体实例化

//电压互感器数据帧，一次读8个寄存器，已包含CRC校验
uint8_t ModbusFrame[]={0x01,0x03,0x00,0x48,0x00,0x08,0xC4,0x1A};

/* CRC校验函数 */
uint16_t calculateCRC(uint8_t* data, uint8_t length) {
	uint16_t crc = 0xFFFF;
	for (int i = 0; i < length; i++) {
		crc ^= data[i];
		for (int j = 0; j < 8; j++) {
			if (crc & 0x0001)
				crc = (crc >> 1) ^ 0xA001;  // CRC-16 MODBUS算法
			else
				crc = crc >> 1;
		}
	}
	return crc;
}
uint16_t combine_bytes(unsigned char high, unsigned char low) {
    return (((unsigned short)high) << 8) | low;
}

void ConvertBytesToUInt32Array(uint8_t* data, uint32_t* output) {
		for (int i = 0; i < 8; i++) {
			output[i] = ((uint32_t)data[i * 4] << 24) |
					((uint32_t)data[i * 4 + 1] << 16) |
					((uint32_t)data[i * 4 + 2] << 8) |
					(uint32_t)data[i * 4 + 3];
		}
}
void Get_AC_Param(uint8_t *frame){
   uint32_t output[8]={0};
   ConvertBytesToUInt32Array(frame, output);
   g_AC_IN.U=output[0]*0.0001f;
   g_AC_IN.I=output[1]*0.0001f;
   g_AC_IN.P=output[2]*0.0001f;
   g_AC_IN.PF=output[4]*0.001f;
   g_AC_IN.F=output[7]*0.01f;




//   printf("[IM1281B] CRC Success\n");
//   printf("[IM1281B] U_in=%f\n", g_AC_IN.U);
//   printf("[IM1281B] I_in=%f\n", g_AC_IN.I);
//   printf("[IM1281B] P_in=%f\n", g_AC_IN.P);
//   printf("[IM1281B] PF_in=%f\n", g_AC_IN.PF);
//   printf("[IM1281B] F_in=%f\n", g_AC_IN.F);
   	 OLED_Printf(0, 10, 16, "[IM1281B]U=%f\n", g_AC_IN.U);
   	 OLED_Reflash();
}

void ModBus_Send(void){

	HAL_UART_Transmit(&huart2, ModbusFrame, sizeof(ModbusFrame), 100);
	HAL_Delay(250);// ??定要延时250ms以上

}

void ModBus_Receive(){
	uint16_t receivedCRC=combine_bytes(g_uart_rx_buffer[36], g_uart_rx_buffer[35]);//校验位合并
	uint8_t rxData[32]={0};
	uint8_t CRCdata[35]={0};
	for(int j=0;j<35;j++){
		CRCdata[j]=g_uart_rx_buffer[j];
	}
	if (g_uart_frame_received) {
		g_uart_frame_received = 0;
//		for (int i = 0; i < FRAME_RECEIVE_LENGTH; i++) {
//			printf("Byte %d: %02X\n",i,g_uart_rx_buffer[i]);//打印接收的数 ???
//		}
		if( calculateCRC(CRCdata, sizeof(CRCdata)== receivedCRC)){
			for(int i=0;i<32;i++){
				rxData[i]=g_uart_rx_buffer[i+3];
			}
			Get_AC_Param(rxData);
			g_get_ac_param_flag =1;
			// 解析接收到的16字节数据
		}
		else{
//			   printf("[IM1281B] U_in=%f\n", g_AC_IN.U);
//			   printf("[IM1281B] I_in=%f\n", g_AC_IN.I);
//			   printf("[IM1281B] P_in=%f\n", g_AC_IN.P);
//			   printf("[IM1281B] PF_in=%f\n", g_AC_IN.PF);
//			   printf("[IM1281B] F_in=%f\n", g_AC_IN.F);
//			printf("[IM1281B] CRC ERROR\n");
			OLED_Printf(0, 48, 16, "[IM1281B] CRC ERROR\n");
			OLED_Reflash();
		}
	}
	// 清空缓冲 ??
	memset(g_uart_rx_buffer, 0, FRAME_RECEIVE_LENGTH);
}

void Show_AC_Param(void){
	ModBus_Send();
	ModBus_Receive();

}

