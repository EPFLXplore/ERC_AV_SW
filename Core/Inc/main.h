/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#include "ADS1113.h"
#include "bno055_stm32.h"
#include "hx711.h"
#include "stemma.h"
#include "vl53l1_api.h"

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
#define GPIO_AUX_1_Pin GPIO_PIN_14
#define GPIO_AUX_1_GPIO_Port GPIOD
#define SPI3_CS1_Pin GPIO_PIN_15
#define SPI3_CS1_GPIO_Port GPIOA
#define SPI3_CS0_Pin GPIO_PIN_4
#define SPI3_CS0_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */
#define HX711_CLK_GPIO_Port GPIOA
#define HX711_DATA_GPIO_Port GPIOA
#define HX711_DATA_Pin 1
#define HX711_CLK_Pin 2
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
