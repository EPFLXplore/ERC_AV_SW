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
#include "i2c.h"
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

/* USER CODE END Variables */
osThreadId GetAccHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void GetAcceleration(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

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
//	initCortex();
  /* Create the thread(s) */
  /* definition and creation of GetAcc */
  osThreadDef(GetAcc, GetAcceleration, osPriorityNormal, 0, 128);
  GetAccHandle = osThreadCreate(osThread(GetAcc), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_GetAcceleration */
/**
  * @brief  Function implementing the GetAcc thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_GetAcceleration */

void GetAcceleration(void const * argument)
{
	 /* USER CODE BEGIN GetAcceleration */
//			char text[100];
//			static IMUData data;
//			static avionics_IMU_packet packet;
//			uint8_t* accel = {0};
//			bno055_assignI2C(&hi2c4);
//			bno055_setup();
//			bno055_setOperationModeNDOF();
			for (;;)
			{
				osDelay(1);
				/* USER CODE END WHILE */
//				data.accel = bnoVectorToVector(bno055_getVectorAccelerometer());
//				if(HAL_I2C_GetError(&hi2c4) == HAL_I2C_ERROR_NONE) {
//			data.toArray((uint8_t*) &packet);
//			network.receive(1,accel,32);
			}
//			osDelay(500);
		//		data.gyro = bnoVectorToVector(bno055_getVectorEuler());
		//		data.mag = bnoVectorToVector(bno055_getVectorGravity());
		//		bno055_vector_t v = bno055_getVectorEuler();
		//		sprintf(text, "Heading: %.2f Roll: %.2f Pitch: %.2f\r\n", v.x, v.y, v.z);
		//		HAL_UART_Transmit(&huart2, text, strlen((char*)text), 0xFF);
		//		v = bno055_getVectorQuaternion();
		//		sprintf(text, "W: %.2f X: %.2f Y: %.2f Z: %.2f\r\n", v.w, v.x, v.y, v.z);
		//		HAL_UART_Transmit(&huart2, text, strlen((char*)text), 0xFF);
		//		bno055_calibration_state_t u = bno055_getCalibrationState();
		//		sprintf(text, "sys: %.2f accel: %.2f gyro: %.2f mag: %.2f\r\n", u.sys, u.accel, u.gyro, u.mag);
		//		HAL_UART_Transmit(&huart2, text, strlen((char*)text), 0xFF);

				/* USER CODE BEGIN 3 */

	  /* USER CODE END GetAcceleration */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
