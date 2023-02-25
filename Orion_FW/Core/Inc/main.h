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
#define SPI2_CS_Pin GPIO_PIN_1
#define SPI2_CS_GPIO_Port GPIOC
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define HAT3_P6_Pin GPIO_PIN_4
#define HAT3_P6_GPIO_Port GPIOC
#define HAT3_P5_Pin GPIO_PIN_5
#define HAT3_P5_GPIO_Port GPIOC
#define HAT3_P3_Pin GPIO_PIN_0
#define HAT3_P3_GPIO_Port GPIOB
#define HAT3_P4_Pin GPIO_PIN_1
#define HAT3_P4_GPIO_Port GPIOB
#define SEL0_Pin GPIO_PIN_12
#define SEL0_GPIO_Port GPIOE
#define SEL1_Pin GPIO_PIN_13
#define SEL1_GPIO_Port GPIOE
#define SEL2_Pin GPIO_PIN_14
#define SEL2_GPIO_Port GPIOE
#define STATUS_5V_Pin GPIO_PIN_11
#define STATUS_5V_GPIO_Port GPIOD
#define HAT2_P1_Pin GPIO_PIN_12
#define HAT2_P1_GPIO_Port GPIOD
#define HAT2_P2_Pin GPIO_PIN_13
#define HAT2_P2_GPIO_Port GPIOD
#define HAT2_P3_Pin GPIO_PIN_14
#define HAT2_P3_GPIO_Port GPIOD
#define HAT1_P2_Pin GPIO_PIN_7
#define HAT1_P2_GPIO_Port GPIOC
#define HAT1_P3_Pin GPIO_PIN_8
#define HAT1_P3_GPIO_Port GPIOC
#define HAT1_P4_Pin GPIO_PIN_9
#define HAT1_P4_GPIO_Port GPIOA
#define HAT1_P6_Pin GPIO_PIN_11
#define HAT1_P6_GPIO_Port GPIOA
#define HAT1_P5_Pin GPIO_PIN_12
#define HAT1_P5_GPIO_Port GPIOA
#define SPI3_CS_Pin GPIO_PIN_15
#define SPI3_CS_GPIO_Port GPIOA
#define LED_YELLOW_Pin GPIO_PIN_2
#define LED_YELLOW_GPIO_Port GPIOD
#define HAT2_P5_Pin GPIO_PIN_4
#define HAT2_P5_GPIO_Port GPIOD
#define HAT2_P6_Pin GPIO_PIN_5
#define HAT2_P6_GPIO_Port GPIOD
#define HAT2_P4_Pin GPIO_PIN_6
#define HAT2_P4_GPIO_Port GPIOD
#define LED_GREEN_Pin GPIO_PIN_7
#define LED_GREEN_GPIO_Port GPIOD
#define HAT3_P1_Pin GPIO_PIN_6
#define HAT3_P1_GPIO_Port GPIOB
#define HAT3_P2_Pin GPIO_PIN_7
#define HAT3_P2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
