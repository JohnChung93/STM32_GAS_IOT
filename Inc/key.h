#ifndef _KEY_H
#define _KEY_H

#include "main.h"

#define KEY0        HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin)   //KEY0 PC5
#define KEY1        HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin) 	//KEY1 PA15
#define WK_UP       HAL_GPIO_ReadPin(WK_UP_GPIO_Port,WK_UP_Pin) //WKUP PA0

#define KEY0_PRES 	1
#define KEY1_PRES		2
#define WKUP_PRES   3

uint8_t KEY_Scan(uint8_t mode);

#endif
