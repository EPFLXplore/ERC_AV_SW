/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "cmsis_os.h"
<<<<<<< Updated upstream:MCU_SOFTWARE/Core/Src/main.cpp
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "Voltmeter_extender.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

=======
//#include "dma.h"
#include "i2c.h"
#include "fdcan.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "stm32h7xx_hal_tim.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "stdio.h"
//#include "cpp_main.h"
>>>>>>> Stashed changes:Orion_FW/Core/Src/main.cpp
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
void PeriphCommonClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

<<<<<<< Updated upstream:MCU_SOFTWARE/Core/Src/main.cpp
=======
  /* MPU Configuration--------------------------------------------------------*/
//  MPU_Config();

  /* Enable I-Cache---------------------------------------------------------*/
//  SCB_EnableICache();
//
//  /* Enable D-Cache---------------------------------------------------------*/
//  SCB_EnableDCache();

>>>>>>> Stashed changes:Orion_FW/Core/Src/main.cpp
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
//  HAL_NVIC_SetPriority(SysTick_IRQn,5,0);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
<<<<<<< Updated upstream:MCU_SOFTWARE/Core/Src/main.cpp
  MX_I2C1_Init();
  MX_DMA_Init();
=======
//  MX_DMA_Init();
>>>>>>> Stashed changes:Orion_FW/Core/Src/main.cpp
  MX_I2C2_Init();
  MX_I2C4_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
<<<<<<< Updated upstream:MCU_SOFTWARE/Core/Src/main.cpp
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_USART6_UART_Init();
  MX_TIM1_Init();
  MX_TIM15_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
  /* USER CODE END 2 */
=======
  MX_UART4_Init();
  MX_UART5_Init();
  MX_UART8_Init();
  MX_USART2_UART_Init();
  MX_FDCAN1_Init();
  MX_I2C1_Init();
>>>>>>> Stashed changes:Orion_FW/Core/Src/main.cpp

//
//  /* Modbus Master initialization */
//    //ModbusH.uModbusType = MB_SLAVE;
//   /* Master initialization */
////   ModbusH.uModbusType = MB_MASTER;
////   ModbusH.port =  &huart2;
////   ModbusH.u8id = 0; // For master it must be 0
////   ModbusH.u16timeOut = 1000;
////   ModbusH.EN_Port1 = GPIOD;
////   ModbusH.EN_Pin1 = GPIO_PIN_4;
////   ModbusH.EN_Port2 = GPIOD;
////   ModbusH.EN_Pin2 = GPIO_PIN_14;
////   ModbusH.u16regs = ModbusDATA;
////   ModbusH.u16regsize= sizeof(ModbusDATA)/sizeof(ModbusDATA[0]);
////   ModbusH.xTypeHW = USART_HW;
////   //Initialize Modbus library
////   ModbusInit(&ModbusH);
////   //Start capturing traffic on serial Port
////   ModbusStart(&ModbusH);
//  /* USER CODE END 2 */
//
//  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
//////
//////  /* Start scheduler */
  osKernelStart();
//
//  /* We should never get here as control is now taken by the scheduler */
//  /* Infinite loop */
//  /* USER CODE BEGIN WHILE */
  while (1)
  {

//	  if(tim7_flag && count < 100000){
//		  tim7_flag = 0;
//		  TxData[0] = count >> 8 & 0xFF;
//
//		  ++cnt_100us;
//		  cnt_500us = cnt_100us / 5;
//		  if(cnt_500us && (cnt_100us%5==0)){
//			  switch(cnt_500us){
//			  	  case 1: fdcan1_transmit(0x108, FDCAN_DLC_BYTES_64, TxData);
//			  	  	  	  break;
//			  	  case 17: fdcan1_transmit(0x003, FDCAN_DLC_BYTES_64, TxData);
//			  	  case 18: break;
//			  	  case 19: break;
//			  	  case 20: ++count; cnt_100us = 0; break;
//			  }
//		  } else {
//			  if(fdcan1_send_fail.flag && cnt_500us > 16){
//				  for(i = 0; i < fdcan1_send_fail.index; i++) {
//					  fdcan1_transmit(fdcan1_send_fail.TxHeader[i].Identifier,
//							  	  	  fdcan1_send_fail.TxHeader[i].DataLength,
//									  &fdcan1_send_fail.TxData[64*i]);
//				  }
//				  fdcan1_send_fail.index = 0;
//				  fdcan1_send_fail.flag = 0;
//			  }
//		  }
//	  }
//	  TxData[0] = 0x69;
//	  			TxData[1] = 0xAD;
//	  			for(uint8_t i = 0; i < 8; i++){
//	  				/* Start the transmission process*/
//	  				TxData[2] = i;
//	  				if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData) != HAL_OK)
//	  				{
//	  					TxData[1] = i;
//	  					/*Transmission request Error*/
//	  					Error_Handler();
//	  				}
////	  				osDelay(10);
//
//	  			}

//	  			}
//	  			if (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0) > 0) {
//	  				/* Retrieve Rx messages from RX FIFO0 */
//	  				HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &RxHeader, RxData);
//	  				HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &RxHeader, &RxData[8]);

//	  if(tim7_flag && count < 1000) {
//		  tim7_flag = 0;
//		  TxData1[0] = count >> 8 & 0xFF;
//		  TxData1[1] = count & 0xFF;
//
//		  ++cnt_100us;
//		  cnt_500us = cnt_100us / 5;
//		  if(cnt_500us && (cnt_100us&5==0)){
//			  switch(cnt_500us){
//			  case 1: fdcan1_transmit(0x108, FDCAN_DLC_BYTES_64, TxData1);
//			  fdcan1_transmit(0x101, FDCAN_DLC_BYTES_64, TxData1);
//			  fdcan1_transmit(0x102, FDCAN_DLC_BYTES_64, TxData1);
//			  fdcan1_transmit(0x103, FDCAN_DLC_BYTES_64, TxData1);
//			  fdcan1_transmit(0x104, FDCAN_DLC_BYTES_64, TxData1);
//			  fdcan1_transmit(0x105, FDCAN_DLC_BYTES_64, TxData1);
//			  fdcan1_transmit(0x106, FDCAN_DLC_BYTES_64, TxData1);
//			  fdcan1_transmit(0x107, FDCAN_DLC_BYTES_64, TxData1);
//			  fdcan1_transmit(0x12345678, FDCAN_DLC_BYTES_64, TxData1);
//			  fdcan1_transmit(0x12345671, FDCAN_DLC_BYTES_64, TxData1);
//			  fdcan1_transmit(0x12345672, FDCAN_DLC_BYTES_64, TxData1);
//			  fdcan1_transmit(0x12345673, FDCAN_DLC_BYTES_64, TxData1);
//			  fdcan1_transmit(0x12345674, FDCAN_DLC_BYTES_64, TxData1);
//			  fdcan1_transmit(0x12345675, FDCAN_DLC_BYTES_64, TxData1);
//			  fdcan1_transmit(0x12345676, FDCAN_DLC_BYTES_64, TxData1);
//			  fdcan1_transmit(0x12345677, FDCAN_DLC_BYTES_64, TxData1);
//			  break;
//			  case 17: break;
//			  case 18: break;
//			  case 20: ++count; cnt_100us = 0; break;
//			  }
//		  } else {
//			  if(fdcan1_send_fail.flag && cnt_500us > 16){
//				  for(i = 0; i < fdcan1_send_fail.index; i++) {
//					  fdcan1_transmit(fdcan1_send_fail.TxHeader[i].Identifier,
//							  	  	  fdcan1_send_fail.TxHeader[i].DataLength,
//									  &fdcan1_send_fail.TxData[64*i]);
//				  }
//				  fdcan1_send_fail.index = 0;
//				  fdcan1_send_fail.flag = 0;
//			  }
//		  }
	  }
    /* USER CODE END WHILE */
	 }
    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSI);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
<<<<<<< Updated upstream:MCU_SOFTWARE/Core/Src/main.cpp
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
=======
  RCC_OscInitStruct.HSICalibrationValue = 32;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 3;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 12;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
>>>>>>> Stashed changes:Orion_FW/Core/Src/main.cpp
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInitStruct.PLL2.PLL2M = 4;
  PeriphClkInitStruct.PLL2.PLL2N = 9;
  PeriphClkInitStruct.PLL2.PLL2P = 4;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_3;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 3072;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

<<<<<<< Updated upstream:MCU_SOFTWARE/Core/Src/main.cpp
/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

=======
>>>>>>> Stashed changes:Orion_FW/Core/Src/main.cpp
/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }

  if (htim->Instance == TIM1) {
	  static int i = 0;
	  i++;
	  if (i == VOLTMETER_MAX_EXTENSION_INTERVAL_MS) {
		  i = 0;
		  HAL_TIM_Base_Stop_IT(&htim1);
		  stop_extender(GPIOA, GPIO_PIN_4, GPIOA, GPIO_PIN_5, get_extender_status());
	  }
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
