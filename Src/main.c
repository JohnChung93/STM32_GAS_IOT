/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body wifi
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "key.h"
#include "delay.h"
#include "lcd.h"
#include "ESP8266.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char THINK_SPEAK_KEY[20] = "xxxxxxxxxxxxxxxx";		//Think speak key
char WIFI_SSID[20] = "xxxxx";											//wifi ssid
char WIFI_PASSWORD[20] = "xxxxxxxx";							//wifi password

int sensorValue = 0;
int gas_enable = 0;

float sensor_volt;
float RS_gas; // Get value of RS in a GAS
float ratio; // Get ratio RS_GAS/RS_air
float R0;  // Get the value of R0 via in LPG
float RS_air; //  Get the value of RS via in a clear air

uint8_t Senser_Init_FLAG = 0;
uint8_t UPLOAD_FLAG = 0;
uint8_t GAS_DETECT_FLAG = 0;

uint8_t UART_Result;

ESP8266_StateTypeDef ESP8266_STATUS = ESP8266_STATE_READY;
WIFI_StateTypeDef WIFI_STATUS = ESP8266_STATE_DISCONNECT;


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t key=0;
	
	uint16_t adcx;
	uint16_t temp_int;
	float temp;
	float temp_radio;
	float temp_R0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	delay_init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim1);
	
	if(LCD_ENABLE){
		LCD_Init();
	}
	
	POINT_COLOR=BLUE;//Set the font to blue 
	
	GAS_Upload_Init();
	
	//Display gas data 
	LCD_ShowString_User(30,130,200,16,16,(uint8_t*)"ADC_CH1_VAL:");	      
	LCD_ShowString_User(30,150,200,16,16,(uint8_t*)"ADC_CH1_VOL:0.000V");
	LCD_ShowString_User(30,170,200,16,16,(uint8_t*)"Please wait senser 5'min");
	LCD_ShowString_User(30,190,200,16,16,(uint8_t*)"When init success");
	LCD_ShowString_User(30,210,200,16,16,(uint8_t*)"Key_0: Let senser init");
	LCD_ShowString_User(30,230,200,16,16,(uint8_t*)"Key_1: Start detect");
	LCD_ShowString_User(30,250,200,16,16,(uint8_t*)"WK_UP: Upload data");
	//notice: To you must initialize senser before start detect 
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		adcx=Get_Adc_Average(ADC_CH1,10);
		LCD_ShowxNum(126,130,adcx,4,16,0);//Display ADC value
		temp=(float)adcx*(3.3/4096);
		adcx=temp;
		LCD_ShowxNum(126,150,adcx,1,16,0);//Display voltage value
		temp-=adcx;
		temp*=1000;

		if(gas_enable){
	    sensor_volt=(float)temp/1000;
			RS_gas = (3.3-sensor_volt)/sensor_volt; // omit *RL
			ratio = RS_gas/R0;
			
			LCD_ShowxNum(78,170,R0,1,16,0);//Display R0 value
			temp_R0 = R0;
			temp_int = temp_R0;
			temp_R0 -= temp_int;
			temp_R0 *= 1000;
			LCD_ShowxNum(94,170,temp_R0,3,16,0x80);
			
			LCD_ShowxNum(78,190,ratio,2,16,0);//Display Rs/R0 value
			temp_radio = ratio;
			temp_int = temp_radio;
			temp_radio -= temp_int;
			temp_radio *= 1000;
			LCD_ShowxNum(102,190,temp_radio,3,16,0x80);
		}

		key=KEY_Scan(0);		//Get button key
		switch(key)
		{				 
			case KEY0_PRES:
				//Initialize gas senser
				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin,GPIO_PIN_RESET);
				LCD_ShowString_User(30,170,200,16,16,(uint8_t*)"R0:   x.xxx              ");
				LCD_ShowString_User(30,190,200,16,16,(uint8_t*)"Rs/R0:xx.xxx             ");
				LCD_ShowString_User(30,210,200,16,16,(uint8_t*)"                         ");
				LCD_ShowString_User(30,230,200,16,16,(uint8_t*)"                         ");
				LCD_ShowString_User(30,250,200,16,16,(uint8_t*)"                         ");
				GAS_Init();
				Senser_Init_FLAG = 1;
				Set_Upload_Count(0);
				UPLOAD_FLAG = 0;
				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin,GPIO_PIN_SET);
				break;
			case KEY1_PRES:
				//Start detect
				if(Senser_Init_FLAG){
					gas_enable = !gas_enable;
				}
				if(gas_enable){
					 HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,GPIO_PIN_RESET);
				}
				else{
					HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,GPIO_PIN_SET);
				}

				break;
			case WKUP_PRES:
				//Upload data to Thing Speak when wifi didn't initialize success wiil restart initialize wifi
				GAS_Data_Upload(ratio,R0,0);
				break;
			default:
				delay_ms(10);	
		}
		
		if(UPLOAD_FLAG && Senser_Init_FLAG){
			GAS_Data_Upload(ratio,R0,1);
		}

		LCD_ShowxNum(142,150,temp,3,16,0X80);
		delay_ms(10);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
  * @brief Gas detect value initialize
  * @retval None
  */
void GAS_Init(void){
	sensorValue = Get_Adc_Average(ADC_CH1,100);

	sensor_volt = (float)sensorValue/4096*3.3;
	RS_air = (3.3-sensor_volt)/sensor_volt; // omit *RL
	R0 = RS_air/9.9; // The ratio of RS/R0 is 9.9 in LPG gas
}

/**
  * @brief ESP8266 initialize for upload gas detect vaule
  * @retval None
  */
void GAS_Upload_Init(void){
	if(WIFI_ENABLE){
		LCD_ShowString_User(30,50,200,16,16,(uint8_t*)"ESP8266 init           ");	
		LCD_ShowString_User(30,70,200,16,16,(uint8_t*)"                       ");	
		
		UART_Result = UART_Transmit_8266(&huart2,"AT+RST","ready",5);
		
		if(UART_Result){
			char buffer_array_mid[10] ="\",\"";
			char buffer_array_end[5] ="\"";
			char data_array[UART_TX_LENGH] ="AT+CWJAP=\"";
			
			strcat(data_array,WIFI_SSID);
			strcat(data_array,buffer_array_mid);
			strcat(data_array,WIFI_PASSWORD);
			strcat(data_array,buffer_array_end);
			
			LCD_ShowString_User(30,50,200,16,16,(uint8_t*)"ESP8266 init success");	
			LCD_ShowString_User(30,70,200,16,16,(uint8_t*)"WiFi connecting");	
			
			UART_Transmit_8266(&huart2,"AT+CWMODE=3","OK",3);
			UART_Result = UART_Transmit_8266(&huart2,data_array,"WIFI GOT IP",10);
			if(UART_Result)
			{
				Set_WIFI_State(ESP8266_STATE_CONNECT);
				LCD_ShowString_User(30,70,200,16,16,(uint8_t*)"WiFi connection success");	
			}
			else{
				Set_WIFI_State(ESP8266_STATE_DISCONNECT);
				LCD_ShowString_User(30,70,200,16,16,(uint8_t*)"WiFi connection fail");	
			}
		}
		else{
			LCD_ShowString_User(30,50,200,16,16,(uint8_t*)"ESP8266 init fail");	
		}
	}
}

/**
  * @brief ESP8266 upload gas value to think speak
  * @retval None
  */
void GAS_Data_Upload(float ratio_upload,float R0_upload,uint8_t auto_flag){
	if(WIFI_ENABLE){
		LCD_ShowString_User(30,110,200,16,16,(uint8_t*)"UART to ESP8266");	
		
		if(WIFI_STATUS == ESP8266_STATE_DISCONNECT && auto_flag == 0){
			GAS_Upload_Init();
		}
		
		
		if(WIFI_STATUS == ESP8266_STATE_CONNECT){
			LCD_ShowString_User(30,90,200,16,16,(uint8_t*)"Upload start...");
			
			char buffer_array[100] ="";
			char data_array[UART_TX_LENGH] ="GET https://api.thingspeak.com/update?api_key=";
			
			sprintf(buffer_array,("&field1=%1.4f&field2=%1.4f"),ratio_upload,R0_upload);
			
			strcat(data_array,THINK_SPEAK_KEY);
			strcat(data_array,buffer_array);
			
			UART_Transmit_8266(&huart2,"AT+CIPSTART=\"TCP\",\"184.106.153.149\",80","OK",3);
			UART_Result = Send_Data_Once_8266(&huart2,data_array,"IPD",3);
			
			if(UART_Result)
			{
				LCD_ShowString_User(30,90,200,16,16,(uint8_t*)"Upload success ");
			}
			else
			{
				LCD_ShowString_User(30,90,200,16,16,(uint8_t*)"Upload Fail    ");
			}
			
		}
		
		LCD_ShowString_User(30,110,200,16,16,(uint8_t*)"               ");	
	}
	Set_Upload_Count(0);
	UPLOAD_FLAG = 0;

}

/**
  * @brief LCD_ShowString with enable flag
  * @retval None
  */
void LCD_ShowString_User(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p){
	if(LCD_ENABLE){
		LCD_ShowString(x,y,width,height,size,p);
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
