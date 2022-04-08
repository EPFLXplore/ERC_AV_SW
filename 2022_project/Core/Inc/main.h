/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI4_CS2_Pin GPIO_PIN_3
#define SPI4_CS2_GPIO_Port GPIOE
#define SPI4_CS1_Pin GPIO_PIN_4
#define SPI4_CS1_GPIO_Port GPIOE
#define SPI1_CS_Pin GPIO_PIN_12
#define SPI1_CS_GPIO_Port GPIOE
#define SPI2_CS0_Pin GPIO_PIN_13
#define SPI2_CS0_GPIO_Port GPIOE
#define SPI2_CS1_Pin GPIO_PIN_14
#define SPI2_CS1_GPIO_Port GPIOE
#define SPI2_CS2_Pin GPIO_PIN_15
#define SPI2_CS2_GPIO_Port GPIOE
#define GPIO_AUX_4_Pin GPIO_PIN_11
#define GPIO_AUX_4_GPIO_Port GPIOD
#define GPIO_AUX_3_Pin GPIO_PIN_12
#define GPIO_AUX_3_GPIO_Port GPIOD
#define GPIO_AUX_2_Pin GPIO_PIN_13
#define GPIO_AUX_2_GPIO_Port GPIOD
#define GPIO_AUX_1_Pin GPIO_PIN_14
#define GPIO_AUX_1_GPIO_Port GPIOD
#define SPI3_CS1_Pin GPIO_PIN_15
#define SPI3_CS1_GPIO_Port GPIOA
#define SPI3_CS0_Pin GPIO_PIN_4
#define SPI3_CS0_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */
#define HX711_CLK_GPIO_Port GPIOD
#define HX711_CLK_Pin GPIO_PIN_2
#define HX711_DATA_GPIO_Port GPIOC
#define HX711_DATA_Pin GPIO_PIN_9

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
