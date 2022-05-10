#ifndef __U087_H
#define __U087_H

#include "stm32h7xx_hal.h"
#include "stdbool.h"
#include "string.h"

#define DEFAULT_TIMEOUT 		1000

#define PAGE512_VOLT            5000.0f

#define ADS115_ADDR 0x49
#define EEPROM_ADDR 0x53

#define ADS1115_RA_CONVERSION   0x00
#define ADS1115_RA_CONFIG       0x01

#define ADS1115_PGA_6144        0x00
#define ADS1115_PGA_4096        0x01
#define ADS1115_PGA_2048        0x02 // default
#define ADS1115_PGA_1024        0x03
#define ADS1115_PGA_512         0x04
#define ADS1115_PGA_256         0x05

#define ADS1115_MV_6144         0.187500F
#define ADS1115_MV_4096         0.125000F
#define ADS1115_MV_2048         0.062500F // default
#define ADS1115_MV_1024         0.031250F
#define ADS1115_MV_512          0.015625F
#define ADS1115_MV_256          0.007813F

#define ADS1115_RATE_8          0x00
#define ADS1115_RATE_16         0x01
#define ADS1115_RATE_32         0x02
#define ADS1115_RATE_64         0x03
#define ADS1115_RATE_128        0x04 // default
#define ADS1115_RATE_250        0x05
#define ADS1115_RATE_475        0x06
#define ADS1115_RATE_860        0x07

#define VOLTMETER_MEASURING_DIR -1

#define ADS1115_MUX_P0N1        0x00 // voltmeter only support

#define ADS1115_COMP_MODE_HYSTERESIS    0x00 // default
#define ADS1115_COMP_MODE_WINDOW        0x01

#define ADS1115_MODE_CONTINUOUS     0x00
#define ADS1115_MODE_SINGLESHOT     0x01 // default

#define VOLTMETER_PRESSURE_COEFFICIENT 0.015918958F

#define VOLTMETER_PAG_6144_CAL_ADDR   208
#define VOLTMETER_PAG_4096_CAL_ADDR   216
#define VOLTMETER_PAG_2048_CAL_ADDR   224
#define VOLTMETER_PAG_1024_CAL_ADDR   232
#define VOLTMETER_PAG_512_CAL_ADDR    240
#define VOLTMETER_PAG_256_CAL_ADDR    248

#define VOLTMETER_FILTER_NUMBER 10
typedef enum {
  PAG_6144 = ADS1115_PGA_6144,
  PAG_4096 = ADS1115_PGA_4096,
  PAG_2048 = ADS1115_PGA_2048, // default
  PAG_1024 = ADS1115_PGA_1024,
  PAG_512 = ADS1115_PGA_512,
  PAG_256 = ADS1115_PGA_256,
} voltmeterGain_t;

typedef enum {
  RATE_8 = ADS1115_RATE_8,
  RATE_16 = ADS1115_RATE_16,
  RATE_32 = ADS1115_RATE_32,
  RATE_64 = ADS1115_RATE_64,
  RATE_128 = ADS1115_RATE_128, // default
  RATE_250 = ADS1115_RATE_250,
  RATE_475 = ADS1115_RATE_475,
  RATE_860 = ADS1115_RATE_860,
} voltmeterRate_t;

typedef enum {
  SINGLESHOT = ADS1115_MODE_SINGLESHOT,
  CONTINUOUS = ADS1115_MODE_CONTINUOUS,
} voltmeterMode_t;


typedef struct {

	I2C_HandleTypeDef *i2cHandle;

	float voltage ;
	float resolution ;
	float hope ;

	uint16_t cover_time ;
	int16_t adc_raw ;

	voltmeterGain_t gain;
	voltmeterRate_t rate;
	voltmeterMode_t mode;
	float calibration_factor;


	int64_t value_test;
} voltmeter;

void voltmeter_init(voltmeter *dev, I2C_HandleTypeDef *i2cHandle);

HAL_StatusTypeDef setGain(voltmeter *dev, voltmeterGain_t gain) ;
HAL_StatusTypeDef setRate(voltmeter *dev, voltmeterRate_t rate) ;
HAL_StatusTypeDef setMode(voltmeter *dev, voltmeterMode_t mode) ;

HAL_StatusTypeDef isInConversion(voltmeter *dev);
HAL_StatusTypeDef startSingleConversion(voltmeter *dev);
HAL_StatusTypeDef getVoltage(voltmeter *dev, bool calibration);

HAL_StatusTypeDef getAdcRaw(voltmeter *dev);
HAL_StatusTypeDef getConversion(voltmeter *dev, uint16_t timeout);

HAL_StatusTypeDef i2cReadBytes(voltmeter *dev, uint8_t addr, uint8_t reg_addr, uint8_t* buff, uint16_t len);
HAL_StatusTypeDef i2cWriteBytes(voltmeter *dev, uint8_t addr, uint8_t reg_addr, uint8_t* buff, uint16_t len);
HAL_StatusTypeDef i2cReadU16(voltmeter *dev, uint8_t addr, uint8_t reg_addr, uint16_t* value);
HAL_StatusTypeDef i2cWriteU16(voltmeter *dev, uint8_t addr, uint8_t reg_addr, uint16_t value);

HAL_StatusTypeDef EEPROMWrite(voltmeter *dev, uint8_t address, uint8_t* buff, uint8_t len);
HAL_StatusTypeDef EEPROMRead(voltmeter *dev, uint8_t address, uint8_t* buff, uint8_t len);
HAL_StatusTypeDef saveCalibration2EEPROM(voltmeter *dev, voltmeterGain_t gain, int16_t hope, int16_t actual);
uint8_t getPGAEEEPROMAddr(voltmeterGain_t gain);
uint16_t getCoverTime(voltmeterRate_t rate);
float getResolution(voltmeterGain_t gain);
HAL_StatusTypeDef readCalibrationFromEEPROM(voltmeter *dev, voltmeterGain_t gain, int16_t* hope, int16_t* actual);




#endif /* __U087_H */
