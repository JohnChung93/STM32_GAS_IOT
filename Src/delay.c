/**
  ******************************************************************************
  * File Name          : Delay.c
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

#include "delay.h"

/**
  * @brief Delay initial (us)
  * @retval None
  */
void delay_init(void){
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000000);
}

/**
  * @brief Delay ms
  * @retval None
  */
void delay_ms(uint32_t Delay){
	HAL_Delay(Delay*1000);
}

/**
  * @brief Delay us
  * @retval None
  */
void delay_us(uint32_t Delay){
	HAL_Delay(Delay);
}
