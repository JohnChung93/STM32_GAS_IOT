/**
  ******************************************************************************
  * File Name          : ESP8266.h
  * Description        : This file provides code for the configuration
  *                      of the Delay instances.
  ******************************************************************************
	* Copyright 2019, John Chung (coffee2235@gmail.com) 
	* 
	* Permission is hereby granted, free of charge, to any person 
	* obtaining a copy of this software and associated documentation 
	* files (the "Software"),to deal in the Software without 
	* restriction, including without limitation the rights to use, 
	* copy, modify, merge, publish, distribute, sublicense, and/or 
	* sell copies of the Software, and to permit persons to whom the 
	* Software is furnished to do so, subject to the following conditions:
	* 
	* The above copyright notice and this permission notice shall be 
	* included in all copies or substantial portions of the Software.
	* 
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
	* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
	* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
	* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH 
	* THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  ******************************************************************************
  */

#ifndef __ESP8266_H
#define __EPS8266_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "String.h"
#include "stdio.h"

	
typedef enum
{
  ESP8266_STATE_READY             = 0x00U,   
  ESP8266_STATE_BUSY              = 0x20U   
}ESP8266_StateTypeDef;

typedef enum
{
  ESP8266_STATE_DISCONNECT        = 0x00U,   
  ESP8266_STATE_CONNECT           = 0x20U   
}WIFI_StateTypeDef;

typedef enum
{
  ESP8266_Upload_Normal        		= 0x00U,   
  ESP8266_Upload_AT_Send 			 	  = 0x20U, 
	ESP8266_Upload_Data 			 	    = 0x24U,   
}ESP8266_UploadMode;

#define UART_TX_LENGH 300
#define UART_RX_LENGH 300

#define UART_CR 0x0D
#define UART_LF 0x0A


uint8_t UART_Transmit_8266( UART_HandleTypeDef* huart_8266,char* data,char* check_data,uint8_t timeout);
uint8_t Send_Data_Once_8266(UART_HandleTypeDef* huart_8266,char* data,char* check_data,uint8_t timeout);
void Set_WIFI_State(WIFI_StateTypeDef state);
uint32_t Count_Array_Lengh(char* data);


#ifdef __cplusplus
}
#endif

#endif 
