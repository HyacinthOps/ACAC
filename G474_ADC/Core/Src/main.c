/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "comp.h"
#include "dac.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "oled.h"
#include "vofa.h"
#include "pid.h"
#include "IM1281B.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define POSITIVE 1
#define NEGATIVE 0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//串口重定向到usart1
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart1,(uint8_t*)&ch, 1, 0xFFFF);
	return ch;
}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint16_t g_count=0;
uint8_t g_H=0;
uint8_t g_L=0;
uint32_t g_time_last=0,g_time_now=0;
uint8_t g_ac_mode;
uint8_t g_pwm_start=0;
uint16_t g_pwm_compare=0;

uint8_t RxBuffer[1];//串口接收缓冲
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



// tim2 other use
//void Delay_us(uint16_t nus)
//{
//	__HAL_TIM_SET_COUNTER(&htim2, 0); //把计数器的�?�设置为0
//	__HAL_TIM_ENABLE(&htim2); //�????????????????????启计�????????????????????
//	while (__HAL_TIM_GET_COUNTER(&htim2) < nus); //每计数一次，就是1us，直到计数器值等于我们需要的时间
//	__HAL_TIM_DISABLE(&htim2); //关闭计数
//
//}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    uint8_t Byte;

    if(UartHandle->Instance==USART2)//如果是串�??????
       {
           //电压反馈
           Byte=g_uart_rx_byte;// ??个一个字节地接收

   		if (g_uart_rx_index == 0) {
   			if (Byte == MODBUS_FRAME_DEVICE_BYTE) {
   				g_uart_rx_buffer[g_uart_rx_index++] = Byte;//�?????? ? 完后再�?�增
   			}
   		}
   		else if(g_uart_rx_index == 1){
   			if (Byte == MODBUS_FRAME_READ_BYTE || Byte == MODBUS_FRAME_WRITE_BYTE) {
   				g_uart_rx_buffer[g_uart_rx_index++] = Byte;//�?????? ? 完后再�?�增
   			}
   			else{//无法识别为CFG服务
   				g_uart_rx_index = 0;
   			}
   		}
   		else {
   			g_uart_rx_buffer[g_uart_rx_index++] = Byte;
   			if (g_uart_rx_index >= FRAME_RECEIVE_LENGTH) {
   				g_uart_rx_index = 0;
   				g_uart_frame_received = 1;
   			}
   		}




           HAL_UART_Receive_IT(&huart2, &g_uart_rx_byte, 1);
       }



}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint8_t Key=0;
	static uint8_t Key_Last=0;

	if(htim==&htim2){
		g_count++;
		HAL_TIM_Base_Stop(&htim2);
		if(HAL_COMP_GetOutputLevel(&hcomp1)==COMP_OUTPUT_LEVEL_HIGH){
			// AC 正半�????????????
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1,3500);
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2,0);
////			//AC负半周的过零
//			HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 3080);
//			HAL_DAC_Start(&hdac1,DAC_CHANNEL_1);//启动DAC1

		}
		else {
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1,0);
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2,3500);
////			//AC正半周的过零
//			HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 3150);
//			HAL_DAC_Start(&hdac1,DAC_CHANNEL_1);//启动DAC1
		}
		g_time_last = g_time_now;
	}

    //按键处理
    if(htim == (&htim4))
    {
        Key = Get_KeyBoard();
        if(Key != Key_Last)
        {
            switch(Key)
            {
                case 0: break;
                case 1:
                    if(g_level == 0)
                        g_suboption0 = (g_suboption0 +1)%4;
                    break;
                case 2:
                    g_suboption0--;
                    if(g_suboption0<0)
                        g_suboption0 = 3;
                    break;
                case 3:
                    g_level = (g_level+1)%2; break;
                case 4:
                    g_level --;
                    if(g_level <0)
                        g_level = 1;
                    break;
                case 9:
                    g_aim_voltage += 0.5;
                    if(g_aim_voltage>35)
                        g_aim_voltage=1;
                    break;
                case 10:
                    g_aim_voltage -=0.5;
                    if(g_aim_voltage<1)
                        g_aim_voltage=35;
                    break;
                case 17:
                	g_pwm_start = ~ g_pwm_start;
//                	if(g_pwm_start){
//                		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
//						HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
//						HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
//						HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);
//                	}
//                	else{
//                		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
//						HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_1);
//						HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
//						HAL_TIMEx_PWMN_Stop(&htim1,TIM_CHANNEL_2);
//                	}

                    break;


            }

        }
       // menu_show(g_level, g_suboption0);
        Key_Last = Key;
    }
}

void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp){
	if(hcomp==&hcomp1){
		g_time_now =  HAL_GetTick();
		if(g_time_now - g_time_last >= 8){
			__HAL_TIM_SET_COUNTER(&htim2, 0);
			HAL_TIM_Base_Start_IT(&htim2);

		}
	}
}




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_COMP1_Init();
  MX_DAC1_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	OLED_Init();//OLED初始�?????????????????????????????????????

	HAL_UARTEx_ReceiveToIdle_IT(&huart1, g_vofa_rx_buffer, VOFA_FRAME_RECEIVE_MAX_LENGTH);
	HAL_UART_Receive_IT(&huart2, &g_uart_rx_byte, 1); //modbus
	HAL_TIM_Base_Start_IT(&htim4);//按键定时器中�??????????????????????????????


	//设置DAC+比较器实�????????????????????????????????????????

	uint32_t VOLTAGE_COMP=0;
	HAL_COMP_Start(&hcomp1);//启动比较�????????????????????????????????????????1
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 3140);//DAC输出2.51V电压  3114
	HAL_DAC_Start(&hdac1,DAC_CHANNEL_1);//启动DAC1
	HAL_Delay(1);

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	g_count=0;

	while (1)
	{

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//		Show_AC_Param();


		OLED_Printf(0, 28, 16, "count=%d",g_count);
		OLED_Reflash();





	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 35;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
