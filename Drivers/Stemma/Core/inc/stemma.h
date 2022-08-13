
#ifndef LIB_SEESAW_H
#define LIB_SEESAW_H

#include "stm32h7xx_hal.h"

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
#define SEESAW_ADDRESS (0x36) ///< Default Seesaw I2C address
/*=========================================================================*/

/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/

enum {
  SEESAW_STATUS_BASE = 0x00,
  SEESAW_GPIO_BASE = 0x01,
  SEESAW_SERCOM0_BASE = 0x02,

  SEESAW_TIMER_BASE = 0x08,
  SEESAW_ADC_BASE = 0x09,
  SEESAW_DAC_BASE = 0x0A,
  SEESAW_INTERRUPT_BASE = 0x0B,
  SEESAW_DAP_BASE = 0x0C,
  SEESAW_EEPROM_BASE = 0x0D,
  SEESAW_NEOPIXEL_BASE = 0x0E,
  SEESAW_TOUCH_BASE = 0x0F,
  SEESAW_KEYPAD_BASE = 0x10,
  SEESAW_ENCODER_BASE = 0x11,
  SEESAW_SPECTRUM_BASE = 0x12,
};

/** GPIO module function address registers
 */
enum {
  SEESAW_GPIO_DIRSET_BULK = 0x02,
  SEESAW_GPIO_DIRCLR_BULK = 0x03,
  SEESAW_GPIO_BULK = 0x04,
  SEESAW_GPIO_BULK_SET = 0x05,
  SEESAW_GPIO_BULK_CLR = 0x06,
  SEESAW_GPIO_BULK_TOGGLE = 0x07,
  SEESAW_GPIO_INTENSET = 0x08,
  SEESAW_GPIO_INTENCLR = 0x09,
  SEESAW_GPIO_INTFLAG = 0x0A,
  SEESAW_GPIO_PULLENSET = 0x0B,
  SEESAW_GPIO_PULLENCLR = 0x0C,
};

/** status module function address registers
 */
enum {
  SEESAW_STATUS_HW_ID = 0x01,
  SEESAW_STATUS_VERSION = 0x02,
  SEESAW_STATUS_OPTIONS = 0x03,
  SEESAW_STATUS_TEMP = 0x04,
  SEESAW_STATUS_SWRST = 0x7F,
};

/** timer module function address registers
 */
enum {
  SEESAW_TIMER_STATUS = 0x00,
  SEESAW_TIMER_PWM = 0x01,
  SEESAW_TIMER_FREQ = 0x02,
};

/** ADC module function address registers
 */
enum {
  SEESAW_ADC_STATUS = 0x00,
  SEESAW_ADC_INTEN = 0x02,
  SEESAW_ADC_INTENCLR = 0x03,
  SEESAW_ADC_WINMODE = 0x04,
  SEESAW_ADC_WINTHRESH = 0x05,
  SEESAW_ADC_CHANNEL_OFFSET = 0x07,
};

/** Sercom module function address registers
 */
enum {
  SEESAW_SERCOM_STATUS = 0x00,
  SEESAW_SERCOM_INTEN = 0x02,
  SEESAW_SERCOM_INTENCLR = 0x03,
  SEESAW_SERCOM_BAUD = 0x04,
  SEESAW_SERCOM_DATA = 0x05,
};

/** neopixel module function address registers
 */
enum {
  SEESAW_NEOPIXEL_STATUS = 0x00,
  SEESAW_NEOPIXEL_PIN = 0x01,
  SEESAW_NEOPIXEL_SPEED = 0x02,
  SEESAW_NEOPIXEL_BUF_LENGTH = 0x03,
  SEESAW_NEOPIXEL_BUF = 0x04,
  SEESAW_NEOPIXEL_SHOW = 0x05,
};

/** touch module function address registers
 */
enum {
  SEESAW_TOUCH_CHANNEL_OFFSET = 0x10,
};

/** keypad module function address registers
 */
enum {
  SEESAW_KEYPAD_STATUS = 0x00,
  SEESAW_KEYPAD_EVENT = 0x01,
  SEESAW_KEYPAD_INTENSET = 0x02,
  SEESAW_KEYPAD_INTENCLR = 0x03,
  SEESAW_KEYPAD_COUNT = 0x04,
  SEESAW_KEYPAD_FIFO = 0x10,
};

/** keypad module edge definitions
 */
enum {
  SEESAW_KEYPAD_EDGE_HIGH = 0,
  SEESAW_KEYPAD_EDGE_LOW,
  SEESAW_KEYPAD_EDGE_FALLING,
  SEESAW_KEYPAD_EDGE_RISING,
};

/** encoder module edge definitions
 */
enum {
  SEESAW_ENCODER_STATUS = 0x00,
  SEESAW_ENCODER_INTENSET = 0x10,
  SEESAW_ENCODER_INTENCLR = 0x20,
  SEESAW_ENCODER_POSITION = 0x30,
  SEESAW_ENCODER_DELTA = 0x40,
};

/** Audio spectrum module function address registers
 */
enum {
  SEESAW_SPECTRUM_RESULTS_LOWER = 0x00, // Audio spectrum bins 0-31
  SEESAW_SPECTRUM_RESULTS_UPPER = 0x01, // Audio spectrum bins 32-63
  // If some future device supports a larger spectrum, can add additional
  // "bins" working upward from here. Configurable setting registers then
  // work downward from the top to avoid collision between spectrum bins
  // and configurables.
  SEESAW_SPECTRUM_CHANNEL = 0xFD,
  SEESAW_SPECTRUM_RATE = 0xFE,
  SEESAW_SPECTRUM_STATUS = 0xFF,
};

#define ADC_INPUT_0_PIN 2 ///< default ADC input pin
#define ADC_INPUT_1_PIN 3 ///< default ADC input pin
#define ADC_INPUT_2_PIN 4 ///< default ADC input pin
#define ADC_INPUT_3_PIN 5 ///< default ADC input pin

#define PWM_0_PIN 4 ///< default PWM output pin
#define PWM_1_PIN 5 ///< default PWM output pin
#define PWM_2_PIN 6 ///< default PWM output pin
#define PWM_3_PIN 7 ///< default PWM output pin

#ifndef INPUT_PULLDOWN
#define INPUT_PULLDOWN                                                         \
  0x03 ///< for compatibility with platforms that do not already define
       ///< INPUT_PULLDOWN
#endif

/*=========================================================================*/

#define SEESAW_HW_ID_CODE_SAMD09 0x55 ///< seesaw HW ID code for SAMD09

#define SEESAW_HW_ID_CODE_TINY8X7 0x87 ///< seesaw HW ID code for ATtiny817

#define SEESAW_EEPROM_I2C_ADDR                                                 \
  0x3F ///< EEPROM address of i2c address to start up with (for devices that
       ///< support this feature)

/**************************************************************************/
/*!
    @brief  Class that stores state and functions for interacting with seesaw
   helper IC
*/
/**************************************************************************/

typedef struct {

	I2C_HandleTypeDef *i2cHandle;

	float temp_C;

	float moisture;

} moist_meter;

uint8_t stemma_init(moist_meter *dev, I2C_HandleTypeDef *i2cHandle);

HAL_StatusTypeDef stemma_ReadTemp(moist_meter *dev);
HAL_StatusTypeDef stemma_ReadMoisture(moist_meter *dev, uint8_t pin);

HAL_StatusTypeDef stemma_ReadRegister(moist_meter *dev, uint8_t reg, uint8_t *data );
HAL_StatusTypeDef stemma_ReadRegisters(moist_meter *dev, uint8_t reg, uint8_t *data, uint8_t length);
HAL_StatusTypeDef stemma_WriteRegister(moist_meter *dev, uint8_t reg, uint8_t *data);
HAL_StatusTypeDef sw_reset(moist_meter *dev);

#endif
