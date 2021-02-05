#ifndef BNO055_STM32_H_
#define BNO055_STM32_H_

#ifdef __cplusplus
  extern "C" {
#endif

#ifdef FREERTOS_ENABLED
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#endif

#include "bno055.h"
#include "stm32h7xx_hal.h"

void bno055_assignI2C(I2C_HandleTypeDef *hi2c_device);

void bno055_delay(int time);

void bno055_writeData(uint8_t reg, uint8_t data);

void bno055_readData(uint8_t reg, uint8_t *data, uint8_t len);

#ifdef __cplusplus
  }
#endif

#endif  // BNO055_STM32_H_
