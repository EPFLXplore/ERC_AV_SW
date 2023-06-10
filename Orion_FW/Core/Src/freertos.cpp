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
#include "System.h"
#include "semphr.h"
<<<<<<< HEAD

#include "Modbus.h"
#include "main.h"
#include "cmsis_os.h"
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
=======
#include "Modbus.h"
>>>>>>> 15e8edf9b101a619741313a75427759da7f1b1e7
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
modbus_t telegram[2];
/* USER CODE END Variables */
/* Definitions for aliveBlink */
osThreadId_t aliveBlinkHandle;
const osThreadAttr_t aliveBlink_attributes = {
  .name = "aliveBlink",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void StartTaskMaster(void *argument);

osThreadId_t myTaskMasterHandle;
const osThreadAttr_t myTaskMaster_attributes = {
  .name = "myTaskMaster",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* USER CODE END FunctionPrototypes */

void AliveBlink(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
<<<<<<< HEAD
	setupTelemetry();
	initCortex();
=======
//	setupTelemetry();
//	initCortex();
>>>>>>> 15e8edf9b101a619741313a75427759da7f1b1e7
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
  /* creation of aliveBlink */
//  aliveBlinkHandle = osThreadNew(AliveBlink, NULL, &aliveBlink_attributes);
<<<<<<< HEAD
//  myTaskMasterHandle = osThreadNew(StartTaskMaster, NULL, &myTaskMaster_attributes);
=======
  myTaskMasterHandle = osThreadNew(StartTaskMaster, NULL, &myTaskMaster_attributes);
>>>>>>> 15e8edf9b101a619741313a75427759da7f1b1e7

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_AliveBlink */
/**
  * @brief  Function implementing the aliveBlink thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_AliveBlink */
void AliveBlink(void *argument)
{
  /* USER CODE BEGIN AliveBlink */
  /* Infinite loop */
<<<<<<< HEAD
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
// 				osDelay(100);
//  /* USER CODE END AliveBlink */
//	  			}
=======
  for(;;)
  {
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
    osDelay(1000);
  }
  /* USER CODE END AliveBlink */
>>>>>>> 15e8edf9b101a619741313a75427759da7f1b1e7
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE BEGIN Header_StartTaskMaster */
/**
* @brief Function implementing the myTaskMaster thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskMaster */
void StartTaskMaster(void *argument)
{
  /* USER CODE BEGIN StartTaskMaster */
  /* Infinite loop */
	 // telegram 0: read registers

  uint32_t u32NotificationValue;

  telegram[0].u8id = 0x01; // slave address
  telegram[0].u8fct = (mb_functioncode_t)3; // function code (this one is registers read)
  //telegram[0].u16RegAdd = 0x160; // start address in slave
  telegram[0].u16RegAdd = 0x0; // start address in slave
  telegram[0].u16CoilsNo = 4; // number of elements (coils or registers) to read
  telegram[0].u16reg = ModbusDATA; // pointer to a memory array in the Arduino


  // telegram 0: read registers
  telegram[1].u8id = 1; // slave address
  telegram[1].u8fct = (mb_functioncode_t)6; // function code (this one is registers write)
  //telegram[1].u16RegAdd = 0x160; // start address in slave
  telegram[1].u16RegAdd = 0x0;
  telegram[1].u16CoilsNo = 1; // number of elements (coils or registers) to read
  telegram[1].u16reg = ModbusDATA; // pointer to a memory array in the Arduino
  int aux;


  for(;;)
  {
	  ModbusQuery(&ModbusH, telegram[0]); // make a query
	  u32NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until query finishes
	  if(u32NotificationValue != ERR_OK_QUERY)
	  {
		//handle error
		//  while(1);
		  aux = 1;
	  }
	  osDelay(10);


//	  ModbusDATA[0]++;
// 	  ModbusQuery(&ModbusH, telegram[1]); // make a query
//	  u32NotificationValue = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // block until query finishes
//	  if(u32NotificationValue)
//	  {
//	  	//handle error
//		//  while(1);
//		  aux =2;
//	  }
//	  osDelay(10);
  }
  /* USER CODE END StartTaskMaster */
}
/* USER CODE END Application */

