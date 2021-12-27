#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "string.h"
#include "DHT.h"
#include "File_Handling_RTOS.h"
#include "stdlib.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "ds3231.h"
//-------------------------------------------//

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
I2C_HandleTypeDef hi2c1;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);


//---------------------------------------------//


//--------------------------------------------GPIO AYARLARI-------------------------------------//
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	 /*Configure GPIO pin : PB3 yesil led */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
}
//--------------------------------------------------------------------------------------------------//

//--------------------------------------------I2C AYARLARI-------------------------------------//
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 32768;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}
//--------------------------------------------------------------------------------------------------//


//------------------------------------------ USART 1 --------------------------------------------//
static void MX_USART1_UART_Init(void)
{

 
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
 

}
//-------------------------------------------------------------------------------------------------//


//------------------------------------------USART 2 -----------------------------------------------//
static void MX_USART2_UART_Init(void)
{

 
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}
//-------------------------------------------------------------------------------------------------//

//----------------------------------------- TIMER 1 -------------------------------//
static void MX_TIM1_Init(void)
{


  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 60000-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 2000-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
 

}
//-------------------------------------------------------------------------------//


//------------------------------------TIMER 3--------------------------------------//
static void MX_TIM3_Init(void)
{

 

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};


  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 60-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0xffff-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
 

}
//---------------------------------------------------------------------------------//


//------------------------------------------SPI 1-------------------------------------//
static void MX_SPI1_Init(void)
{

 
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
 
}
//---------------------------------------------------------------------------------//


//---------------------------------------ADC 1--------------------------------------//
static void MX_ADC1_Init(void)
{

 
  ADC_ChannelConfTypeDef sConfig = {0};


  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
 
}

//---------------------------------------------------------------------------------//

//---------------------------------------ADC 2--------------------------------------//
static void MX_ADC2_Init(void)
{

 
  ADC_ChannelConfTypeDef sConfig = {0};


  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }
  
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
 
}

//---------------------------------------------------------------------------------//


//----------------------------SYSTEM CLOCK CONFIG------------------------------------------//
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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

//---------------------------------------------------------------------------------//
//--------DS3231 Degerleri--------//
uint8_t ds3231Addr = (uint8_t) 0xD0;
uint32_t saniye, dakika, saat;
uint32_t gun, ay, yil;
//-------------------------------//

//--------DHT11 Degerleri--------//
DHT_DataTypedef DHT11_Data;
float Temperature, Humidity;
//-------------------------------//


//--------ACS712 Degerleri-------//
uint16_t adcValue=0;
uint16_t adcValue2=0;
float sensitivity = 66;
float offsetVoltage = 1600;
float adcVoltage = 0;
float currentValue = 0;
float adcVoltage2 = 0;
float ruzgarhizi=0;
float sensitivity2=66;

//-------------------------------//

//------------------------------------------int main-------------------------------------------------------//

int main(void)
{
	//---------girisler----------//
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC1_Init();
	MX_ADC2_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
	MX_USART2_UART_Init();
	MX_I2C1_Init();
	//----------------------------//
  
	
	Mount_SD("/");
  Format_SD();
  Create_File("datalog.txt");
  Unmount_SD("/");
	
 //----------------------Saat Tanimlamasi-----------------------//
 setSeconds(00);
 setMinutes(40);
 setHour(17);
 setDate(29);
 setMonth(12);
 setYear(20);
 
 DS3231_sendData(hi2c1, ds3231Addr);
 while (HAL_I2C_GetState( & hi2c1) != HAL_I2C_STATE_READY){}
 DS3231_setDate(hi2c1, ds3231Addr);
 HAL_Delay(500);
 //--------------------------------------------------------------//
 
 while (1)
 {
	 
	 if(Create_File("datalog.txt")) {

   HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
	 
	while(1)
		 
	{
   //---------------------------DS3231--------------------------//
	 DS3231_sendData(hi2c1, ds3231Addr);
	 while (HAL_I2C_GetState( & hi2c1) != HAL_I2C_STATE_READY) {}
	 I2C_ReadCalendarData(hi2c1, ds3231Addr); 
	 
	 saniye = atoi(readSeconds());
	 dakika = atoi(readMinutes());
	 saat = atoi(readHours());

	 gun = atoi(readDate());
	 ay  = atoi(readMonth());
	 yil = atoi(readYear()); 
	//-----------------------------------------------------------// 
		 
				 
	 char *buffer = pvPortMalloc(100*sizeof(char));
   
	 Mount_SD("/");
		
		//---------------------------DHT11----------------------------------//
		DHT_GetData(&DHT11_Data);
		Temperature = DHT11_Data.Temperature;
	  Humidity = DHT11_Data.Humidity;
		//------------------------------------------------------------------//
		
		
		//-----------------------------ACS712-------------------------------//
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1,10);
		adcValue = HAL_ADC_GetValue(&hadc1);
		adcVoltage = (adcValue / 3882.37498) * 3150.0;
		currentValue = ((adcVoltage - offsetVoltage) / sensitivity);
		HAL_ADC_Stop(&hadc1);
		//------------------------------------------------------------------//
		
		//-----------------------------Anemometre-------------------------------//
		HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2,10);
		adcValue2 = HAL_ADC_GetValue(&hadc2);
		adcVoltage2 = (adcValue2 / 3882.37498) * 3150.0;
		ruzgarhizi = (adcVoltage2 / sensitivity2);
	
		HAL_ADC_Stop(&hadc2);
		//------------------------------------------------------------------//
		
		sprintf (buffer, "%0.2u/%0.2u/%0.2u  %0.2u:%0.2u:%0.2u   Sicaklik =%.1f  Nem=%.1f  Akim=%.2f  Ruzgar Hizi=%.2f km/h  \n",gun,ay,yil,saat,dakika,saniye,Temperature,Humidity,currentValue,ruzgarhizi);
		
		
		Update_File("datalog.txt",buffer);
		vPortFree(buffer);
		Unmount_SD("/");
		
		HAL_UART_Transmit(&huart2,(uint8_t*)buffer,strlen(buffer),100);
		
		
		HAL_Delay(3000);  
		}
}

 }

 vTaskStartScheduler();
}
//---------------------------------------------------------------------------------------------------------//

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
 }

void Error_Handler(void)
{
}

