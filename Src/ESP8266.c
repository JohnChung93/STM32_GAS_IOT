/**
  ******************************************************************************
  * File Name          : ESP8266.c
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

#include "ESP8266.h"
#include "delay.h"	

uint8_t TxByte_8266[UART_TX_LENGH];
uint8_t RxByte_8266[UART_RX_LENGH];

extern ESP8266_StateTypeDef ESP8266_STATUS;
extern WIFI_StateTypeDef WIFI_STATUS;

/**
  * @brief  Transmit Uart to ESP8266 and check return data
  * @param  huart_8266: UART handle current use
						data: Uart to EPS288 data
						check_data: Check ESP8266 Uart return data text
						timeout: Limit time for this Uart
  * @retval Uart result after check data 1,success 0,fail
  */
uint8_t UART_Transmit_8266(UART_HandleTypeDef* huart_8266,char* data,char* check_data,uint8_t timeout)
{
	uint32_t lengh_data = Count_Array_Lengh(data);
	uint32_t check_lengh = Count_Array_Lengh(check_data);
	
	uint8_t status;
	uint8_t check = 0;
	uint32_t check_count = 0;
	
	uint8_t Check_Byte_8266[check_lengh];
	
	uint8_t result = 0;
	
	ESP8266_STATUS = ESP8266_STATE_BUSY;
	
	memset(TxByte_8266,0,sizeof(TxByte_8266));
	memset(RxByte_8266,0,sizeof(RxByte_8266));
	
	for(int i=0;i<lengh_data;i++){
		TxByte_8266[i] = *data;
		data++;
	}
	
	for(int i=0;i<lengh_data;i++){
		Check_Byte_8266[i] = *check_data;
		check_data++;
	}

		
	TxByte_8266[lengh_data] = UART_CR;
	TxByte_8266[lengh_data+1] = UART_LF;
	
	
	status = HAL_UART_Transmit(huart_8266,TxByte_8266,lengh_data+2,0xffff);
		
	if(status == HAL_OK){
		while(check==0 && timeout>0){
			HAL_UART_Receive(huart_8266,RxByte_8266,UART_RX_LENGH,1000*1000);
			
			for(int i=0;i<UART_RX_LENGH;i++){
					if(Check_Byte_8266[check_count] == RxByte_8266[i]){
						check_count++;
						if(check_count == check_lengh){
							check = 1;
							result = 1;
							break;
						}
					}
					else{
						check_count = 0;
					}
			}
			timeout--;
		}
	}
	
	while(huart_8266->gState != HAL_UART_STATE_READY);
	delay_ms(500);
	
	ESP8266_STATUS = ESP8266_STATE_READY;
	return result;
}

/**
  * @brief  Transmit ESP8266 AT+CIPSEND commend and http post data use Uart
  * @param  huart_8266: UART handle current use
						data: AT+CIPSEND http post data
						check_data: Check ESP8266 Uart return data text
						timeout: Limit time for this Uart
  * @retval Uart result after check data 1,success 0,fail
  */
uint8_t Send_Data_Once_8266(UART_HandleTypeDef* huart_8266,char* data,char* check_data,uint8_t timeout)
{
	uint32_t lengh_data = Count_Array_Lengh(data);
	uint8_t result = 0;
	char buffer_array[100] ="";
	
	sprintf(buffer_array,("AT+CIPSEND=%d"),lengh_data+2);

	result = UART_Transmit_8266(huart_8266,buffer_array,"OK",3);
	
	if(result){
		result = UART_Transmit_8266(huart_8266,data,check_data,timeout);
	}
	
	return result;
}

/**
  * @brief  Set WIFI_STATUS sate
  * @param  state: WIFI_StateTypeDef Structure input 
  * @retval None
  */
void Set_WIFI_State(WIFI_StateTypeDef state){
	WIFI_STATUS = state;
}

/**
  * @brief  Count pointer array lengh
  * @param  data: pointer array start data position
  * @retval Calculate Array lengh
  */
uint32_t Count_Array_Lengh(char* data){
	uint32_t lengh = 0;
	while(*data != NULL)
	{
		lengh++;
		data++;
	}
	return lengh;
}

