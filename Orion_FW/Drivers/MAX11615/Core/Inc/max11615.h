
/**
  * @file       max11615.h
  * @author     Simon Burkhardt github.com/mnemocron
  * @copyright  MIT license
  * @date       06.2018
  * @brief      C library for the MAX11615EEE+ I2C ADC for STM32 HAL.
  * @details
  * @see 	      github.com/mnemocron
  * @see 	      https://datasheets.maximintegrated.com/en/ds/MAX11612-MAX11617.pdf
  * @see        https://github.com/AllAboutEE/MAX11609EEE-Breakout-Board/tree/master/Software/Arduino/AllAboutEE-MAX11609-Library
  */

#ifndef MAX11615_H_
#define MAX11615_H_

/**
  * @note 		tested using STM32F373
  */
#ifndef STM32H7XX_H
#include "stm32h7xx_hal.h"
#endif

#ifndef STM32H7XX_HAL_I2C_H
#include "stm32h7xx_hal_i2c.h"
#endif

#ifndef MAIN_H
#include "main.h"
#endif

/** @see Datasheet p.19 Table 6
  */
#define MAX11615_REF_VDD      0x00
#define MAX11615_REF_EXTERNAL 0x02
#define MAX11615_REF_INTERNAL 0x04
#define MAX11615_ANANLOG_IN   0x00
#define MAX11615_REF_OUT      0x02
#define MAX11615_INT_REF_ON   0x01

typedef struct {
	uint16_t devAddress;
	I2C_HandleTypeDef *wireIface;
} MAX11615;

MAX11615 new_MAX11615              (void);

uint8_t MAX11615_Write8            (MAX11615*, uint8_t, uint8_t);
uint8_t MAX11615_Read8             (MAX11615*, uint8_t, uint8_t*);
uint8_t MAX11615_Init              (MAX11615*, I2C_HandleTypeDef*, uint16_t, uint8_t);
uint8_t MAX11615_Setup             (MAX11615*, uint8_t);
uint8_t MAX11615_Configuration 	   (MAX11615*, uint8_t);
uint8_t MAX11615_ADC_Read          (MAX11615*, uint8_t, uint16_t*);
uint8_t MAX11615_Scan              (MAX11615*, uint16_t*);

#endif
