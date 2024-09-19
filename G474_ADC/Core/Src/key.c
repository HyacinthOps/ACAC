#include "key.h"




uint8_t Get_KeyBoard(void)
{
    uint8_t Key_Board=0;

    if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==KEY_ON)     //检测到按键被按下
   {
	  Key_Board = 17;
   }

    HAL_GPIO_WritePin(R1_GPIO_Port,R1_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(R2_GPIO_Port,R2_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R3_GPIO_Port,R3_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R4_GPIO_Port,R4_Pin,GPIO_PIN_RESET);

    if(HAL_GPIO_ReadPin(C1_GPIO_Port,C1_Pin)==KEY_ON)     //检测到按键被按下
    {
       Key_Board = 16;
    }
    else if(HAL_GPIO_ReadPin(C2_GPIO_Port,C2_Pin)==KEY_ON)     //检测到按键被按下
    {
       Key_Board =15;
    }
    else if(HAL_GPIO_ReadPin(C3_GPIO_Port,C3_Pin)==KEY_ON)     //检测到按键被按下
    {
       Key_Board =14;
    }
    else if(HAL_GPIO_ReadPin(C4_GPIO_Port,C4_Pin)==KEY_ON)     //检测到按键被按下
    {
       Key_Board =13;
    }




    HAL_GPIO_WritePin(R1_GPIO_Port,R1_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R2_GPIO_Port,R2_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(R3_GPIO_Port,R3_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R4_GPIO_Port,R4_Pin,GPIO_PIN_RESET);

    if(HAL_GPIO_ReadPin(C1_GPIO_Port,C1_Pin)==KEY_ON)     //检测到按键被按下
    {
       Key_Board =12;
    }
    else if(HAL_GPIO_ReadPin(C2_GPIO_Port,C2_Pin)==KEY_ON)     //检测到按键被按下
    {
       Key_Board =11;
    }
    else if(HAL_GPIO_ReadPin(C3_GPIO_Port,C3_Pin)==KEY_ON)     //检测到按键被按下
    {
       Key_Board =10;
    }
    else if(HAL_GPIO_ReadPin(C4_GPIO_Port,C4_Pin)==KEY_ON)     //检测到按键被按下
    {
       Key_Board =9;
    }


    HAL_GPIO_WritePin(R1_GPIO_Port,R1_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R2_GPIO_Port,R2_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R3_GPIO_Port,R3_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(R4_GPIO_Port,R4_Pin,GPIO_PIN_RESET);

    if(HAL_GPIO_ReadPin(C1_GPIO_Port,C1_Pin)==KEY_ON)     //检测到按键被按下
    {
       Key_Board =8;
    }
    else if(HAL_GPIO_ReadPin(C2_GPIO_Port,C2_Pin)==KEY_ON)     //检测到按键被按下
    {
       Key_Board =7;
    }
    else if(HAL_GPIO_ReadPin(C3_GPIO_Port,C3_Pin)==KEY_ON)     //检测到按键被按下
    {
       Key_Board =6;
    }
    else if(HAL_GPIO_ReadPin(C4_GPIO_Port,C4_Pin)==KEY_ON)     //检测到按键被按下
    {
       Key_Board =5;
    }


    HAL_GPIO_WritePin(R1_GPIO_Port,R1_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R2_GPIO_Port,R2_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R3_GPIO_Port,R3_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R4_GPIO_Port,R4_Pin,GPIO_PIN_SET);

    if(HAL_GPIO_ReadPin(C1_GPIO_Port,C1_Pin)==KEY_ON)     //检测到按键被按下
    {
       Key_Board = 4;
    }
    else if(HAL_GPIO_ReadPin(C2_GPIO_Port,C2_Pin)==KEY_ON)     //检测到按键被按下
    {
       Key_Board =3;
    }
    else if(HAL_GPIO_ReadPin(C3_GPIO_Port,C3_Pin)==KEY_ON)     //检测到按键被按下
    {
       Key_Board =2;
    }
    else if(HAL_GPIO_ReadPin(C4_GPIO_Port,C4_Pin)==KEY_ON)     //检测到按键被按下
    {
       Key_Board =1;
    }

    return Key_Board;

}
