/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "ADS1113.h"
#include "BMP280.hpp"
#include <stdbool.h>
#include "stdio.h"


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
/* USER CODE BEGIN Variables */
ads1113_t voltmeter;
uint8_t ADS_MSG3[100] = {0};
uint8_t ADS_MSG4[100] = {0};
uint8_t ADS_MSG5[100] = {0};
uint8_t ADS_MSG8[100] = {0};
uint8_t BMP_MSG3[100] = {0};
uint8_t BMP_MSG4[100] = {0};
uint8_t BMP_MSG5[100] = {0};
uint8_t BMP_MSG8[100] = {0};
/* USER CODE END Variables */
/* Definitions for blink01 */
osThreadId_t blink01Handle;
const osThreadAttr_t blink01_attributes = {
  .name = "blink01",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for adsTask */
osThreadId_t adsTaskHandle;
const osThreadAttr_t adsTask_attributes = {
  .name = "adsTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for gpioTask */
osThreadId_t gpioTaskHandle;
const osThreadAttr_t gpioTask_attributes = {
  .name = "gpioTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for bmpTask */
osThreadId_t bmpTaskHandle;
const osThreadAttr_t bmpTask_attributes = {
  .name = "bmpTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for blink02 */
osThreadId_t blink02Handle;
const osThreadAttr_t blink02_attributes = {
  .name = "blink02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartBlink01(void *argument);
void StartAdsTask(void *argument);
void StartGPIOTask(void *argument);
void StartBMPTask(void *argument);
void StartBlink02(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of blink01 */
  blink01Handle = osThreadNew(StartBlink01, NULL, &blink01_attributes);

  /* creation of adsTask */
  adsTaskHandle = osThreadNew(StartAdsTask, NULL, &adsTask_attributes);

  /* creation of gpioTask */
  gpioTaskHandle = osThreadNew(StartGPIOTask, NULL, &gpioTask_attributes);

  /* creation of bmpTask */
  bmpTaskHandle = osThreadNew(StartBMPTask, NULL, &bmpTask_attributes);

  /* creation of blink02 */
  blink02Handle = osThreadNew(StartBlink02, NULL, &blink02_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartBlink01 */
/**
  * @brief  Function implementing the blink01 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartBlink01 */
void StartBlink01(void *argument)
{
  /* USER CODE BEGIN StartBlink01 */
  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
    osDelay(1000);
  }
  /* USER CODE END StartBlink01 */
}

/* USER CODE BEGIN Header_StartAdsTask */
/**
* @brief Function implementing the adsTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAdsTask */
void StartAdsTask(void *argument)
{
  /* USER CODE BEGIN StartAdsTask */
  #if defined(TEST_HAT_1)
	bool success = ADS1113_init(&voltmeter, &hi2c1, ADS_ADDR_GND);
	uint8_t hat_number = 1;
  #elif defined(TEST_HAT_2)
	bool success = ADS1113_init(&voltmeter, &hi2c2, ADS_ADDR_GND);
	uint8_t hat_number = 2;
  #elif defined(TEST_HAT_3)
	HAL_I2C_DeInit(&hi2c3);
	bool success = ADS1113_init(&voltmeter, &hi2c3, ADS_ADDR_GND);
	uint8_t hat_number = 3;
  #endif

	ADSsetGain(&voltmeter, GAIN_ONE);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
    if (success) {
		int16_t val = ADSreadADC_Differential_0_1(&voltmeter);
		printf("Differential value %d\n", val);
		sprintf((char*)ADS_MSG3, "from UART3 (HAT %d): Differential value %d \n \r", hat_number, val);
		sprintf((char*)ADS_MSG4, "from UART4 (HAT %d): Differential value %d \n \r", hat_number, val);
		sprintf((char*)ADS_MSG5, "from UART5 (HAT %d): Differential value %d \n \r", hat_number, val);
		sprintf((char*)ADS_MSG8, "from UART8 (HAT %d): Differential value %d \n \r", hat_number, val);
		HAL_UART_Transmit(&huart3, ADS_MSG3, sizeof(ADS_MSG3), 100);
		HAL_UART_Transmit(&huart4, ADS_MSG4, sizeof(ADS_MSG4), 100);
		HAL_UART_Transmit(&huart5, ADS_MSG5, sizeof(ADS_MSG5), 100);
		HAL_UART_Transmit(&huart8, ADS_MSG8, sizeof(ADS_MSG8), 100);
		HAL_UART_Transmit(&huart1, ADS_MSG8, sizeof(ADS_MSG8), 100);
	}
  }
  /* USER CODE END StartAdsTask */
}

/* USER CODE BEGIN Header_StartGPIOTask */
/**
* @brief Function implementing the gpioTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGPIOTask */
void StartGPIOTask(void *argument)
{
  /* USER CODE BEGIN StartGPIOTask */

  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_TogglePin(HAT_P1_GPIO_Port, HAT_P1_Pin);
    osDelay(100);
    HAL_GPIO_TogglePin(HAT_P2_GPIO_Port, HAT_P2_Pin);
    osDelay(100);
    HAL_GPIO_TogglePin(HAT_P3_GPIO_Port, HAT_P3_Pin);
    osDelay(100);
	HAL_GPIO_TogglePin(HAT_P4_GPIO_Port, HAT_P4_Pin);
    osDelay(100);
    HAL_GPIO_TogglePin(HAT_P5_GPIO_Port, HAT_P5_Pin);
    osDelay(100);
    HAL_GPIO_TogglePin(HAT_P6_GPIO_Port, HAT_P6_Pin);
    osDelay(100);
  }
  /* USER CODE END StartGPIOTask */
}

/* USER CODE BEGIN Header_StartBMPTask */
/**
* @brief Function implementing the bmpTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBMPTask */
void StartBMPTask(void *argument)
{
  /* USER CODE BEGIN StartBMPTask */
  osDelay(500);

  #if defined(TEST_HAT_1)
	BMP280 bmp(&hspi1);
	uint8_t hat_number = 1;
  #elif defined(TEST_HAT_2)
	BMP280 bmp(&hspi2);
	uint8_t hat_number = 2;
  #elif defined(TEST_HAT_3)
	BMP280 bmp(&hspi3);
	uint8_t hat_number = 3;
  #endif

  uint8_t status = bmp.initialize();
  /* Infinite loop */
  for(;;)
  {
	bmp.measure();
	printf("Pressure = %f \t Temperature = %f \n", bmp.measurement.pressure, bmp.measurement.temperature);
	sprintf((char*)BMP_MSG3, "from UART3 (HAT %d): Pressure = %f \t Temperature = %f \n \r", hat_number, bmp.measurement.pressure, bmp.measurement.temperature);
	sprintf((char*)BMP_MSG4, "from UART4 (HAT %d): Pressure = %f \t Temperature = %f \n \r", hat_number, bmp.measurement.pressure, bmp.measurement.temperature);
	sprintf((char*)BMP_MSG5, "from UART5 (HAT %d): Pressure = %f \t Temperature = %f \n \r", hat_number, bmp.measurement.pressure, bmp.measurement.temperature);
	sprintf((char*)BMP_MSG8, "from UART8 (HAT %d): Pressure = %f \t Temperature = %f \n \r", hat_number, bmp.measurement.pressure, bmp.measurement.temperature);
	HAL_UART_Transmit(&huart3, BMP_MSG3, sizeof(BMP_MSG3), 100);
	HAL_UART_Transmit(&huart4, BMP_MSG4, sizeof(BMP_MSG4), 100);
	HAL_UART_Transmit(&huart5, BMP_MSG5, sizeof(BMP_MSG5), 100);
	HAL_UART_Transmit(&huart8, BMP_MSG8, sizeof(BMP_MSG8), 100);
	HAL_UART_Transmit(&huart1, BMP_MSG8, sizeof(BMP_MSG8), 100);
	osDelay(1000);
  }
  /* USER CODE END StartBMPTask */
}

/* USER CODE BEGIN Header_StartBlink02 */
/**
* @brief Function implementing the blink02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBlink02 */
void StartBlink02(void *argument)
{
  /* USER CODE BEGIN StartBlink02 */
  /* Infinite loop */
  for(;;)
  {
	HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
	osDelay(600);
  }
  /* USER CODE END StartBlink02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

