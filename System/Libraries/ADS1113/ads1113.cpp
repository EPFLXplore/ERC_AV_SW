/*
 * ADS1113.cpp
 *
 *  Created on: Mar 7, 2021
 *      Author: AV Team
 */

#include <Libraries/ADS1113/ads1113.h>

#define MAX_VALUE 32768

void ADS1113::ads1113_delay(int time){
#ifdef FREERTOS_ENABLED
  osDelay(time);
#else
  HAL_Delay(time);
#endif
}

ADS1113::ADS1113(I2C_HandleTypeDef *hi2c, uint8_t i2cAddress, float multiplier):
	_i2cAddress(i2cAddress),
	_conversionDelay(ADS1113_CONVERSIONDELAY),
	_bitShift(0),
	_ads1113_i2c_port(hi2c),
	_multiplier(multiplier / (MAX_VALUE >> _bitShift))
{}

bool ADS1113::begin(){
	if (HAL_I2C_Init(_ads1113_i2c_port) == HAL_OK){
		if(HAL_I2C_IsDeviceReady(_ads1113_i2c_port, (_i2cAddress<<1), 10, 100) == HAL_OK)
			return true;
	}
	return false;
}

/* Write to register in ADS1113 directly */
static void writeRegister(I2C_HandleTypeDef* i2c_port, uint16_t i2cAddress, uint8_t reg, uint16_t value) {
	uint8_t pData[2];
	pData[0] = (value >> 8);
	pData[1] = value & 0xff;

	while(HAL_I2C_IsDeviceReady(i2c_port, i2cAddress << 1, 10, 10) != HAL_OK);
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(i2c_port, i2cAddress << 1, reg, I2C_MEMADD_SIZE_8BIT, pData, 2, 10);
}

/* Read from register in ADS1113 directly */
static uint16_t readRegister(I2C_HandleTypeDef* i2c_port, uint16_t i2cAddress, uint8_t reg) {
	uint8_t pData[2];

	while(HAL_I2C_IsDeviceReady(i2c_port, i2cAddress << 1, 10, 10) != HAL_OK);
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(i2c_port, i2cAddress << 1, reg, I2C_MEMADD_SIZE_8BIT, pData, 2, 10);
	uint16_t regData = ((pData[0] << 8) | pData[1]);
	return regData; //CHECK HERE IF THERE ARE ERRORS : inverse pData[0] and pData[1]
}

int16_t ADS1113::readADC_SingleEnded(uint16_t sampleRate) {
  // Start with default values
  uint16_t config =
      ADS1X15_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
      ADS1X15_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
      ADS1X15_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
      ADS1X15_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
      sampleRate 					|   // 1600 samples per second (default)
      ADS1X15_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set 'start single-conversion' bit
  config |= ADS1X15_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  writeRegister(_ads1113_i2c_port, _i2cAddress, ADS1X15_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  ads1113_delay(_conversionDelay);

  // Read the conversion results
  // Shift 12-bit results right 4 bits for the ADS1X15
  return readRegister(_ads1113_i2c_port, _i2cAddress, ADS1X15_REG_POINTER_CONVERT) >> _bitShift;
}

int16_t ADS1113::readADC_Differential_0_1(uint16_t sampleRate) {
  // Start with default values
  uint16_t config =
      ADS1X15_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
      ADS1X15_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
      ADS1X15_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
      ADS1X15_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
      ADS1X15_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set data rate
  config |= sampleRate;

  // Set channels
  config |= ADS1X15_REG_CONFIG_MUX_DIFF_0_1; // AIN0 = P, AIN1 = N

  // Set 'start single-conversion' bit
  config |= ADS1X15_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  writeRegister(_ads1113_i2c_port, _i2cAddress, ADS1X15_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  while (!conversionComplete())
    ;

  // Read the conversion results
  return getLastConversionResults();
}

int16_t ADS1113::getLastConversionResults() {
	// Wait for the conversion to complete
	ads1113_delay(_conversionDelay);

	// Read the conversion results
	uint16_t res = readRegister(_ads1113_i2c_port, _i2cAddress, ADS1X15_REG_POINTER_CONVERT) >> _bitShift;
	if (_bitShift == 0) {
		return (int16_t)res;
	}
	else {
	// Shift 12-bit results right 4 bits for the ADS1X15,
	// making sure we keep the sign bit intact
		if (res > 0x07FF) {
			// negative number - extend the sign to 16th bit
			res |= 0xF000;
		}
	return (int16_t)res;
	}
}

bool ADS1113::conversionComplete() {
  return (readRegister(_ads1113_i2c_port, _i2cAddress, ADS1X15_REG_POINTER_CONFIG) & 0x8000) != 0;
}

float ADS1113::getMultiplier(){
	return _multiplier;
}

ADS1115::ADS1115(I2C_HandleTypeDef *hi2c, uint8_t i2cAddress):
	ADS1113(hi2c, i2cAddress, 6.144f),
	_gain(GAIN_TWOTHIRDS)
	/* +/- 6.144V range (limited to VDD +0.3V max!) */
{}

void ADS1115::startComparator_SingleEnded(uint8_t channel, int16_t threshold, uint16_t sampleRate) {
  // Start with default values
  uint16_t config =
      ADS1X15_REG_CONFIG_CQUE_1CONV |   // Comparator enabled and asserts on 1
                                        // match
      ADS1X15_REG_CONFIG_CLAT_LATCH |   // Latching mode
      ADS1X15_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
      ADS1X15_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
      sampleRate					 |  // 1600 samples per second (default)
      ADS1X15_REG_CONFIG_MODE_CONTIN;	// Single-shot mode (default)

  // Set PGA/voltage range
  config |= _gain;

  // Set single-ended input channel
  switch (channel) {
  case (0):
    config |= ADS1X15_REG_CONFIG_MUX_SINGLE_0;
    break;
  case (1):
    config |= ADS1X15_REG_CONFIG_MUX_SINGLE_1;
    break;
  case (2):
    config |= ADS1X15_REG_CONFIG_MUX_SINGLE_2;
    break;
  case (3):
    config |= ADS1X15_REG_CONFIG_MUX_SINGLE_3;
    break;
  }

  // Set the high threshold register
  // Shift 12-bit results left 4 bits for the ADS1X15
  writeRegister(_ads1113_i2c_port, _i2cAddress, ADS1X15_REG_POINTER_HITHRESH, threshold << _bitShift);

  // Write config register to the ADC
  writeRegister(_ads1113_i2c_port, _i2cAddress, ADS1X15_REG_POINTER_CONFIG, config);
}

int16_t ADS1115::readADC_SingleEnded(uint8_t channel, uint16_t sampleRate) {
  if (channel > 3) {
    return 0;
  }

  // Start with default values
  uint16_t config =
      ADS1X15_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
      ADS1X15_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
      ADS1X15_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
      ADS1X15_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
      sampleRate |   // 1600 samples per second (default)
      ADS1X15_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= _gain;

  // Set single-ended input channel
  switch (channel) {
  case (0):
    config |= ADS1X15_REG_CONFIG_MUX_SINGLE_0;
    break;
  case (1):
    config |= ADS1X15_REG_CONFIG_MUX_SINGLE_1;
    break;
  case (2):
    config |= ADS1X15_REG_CONFIG_MUX_SINGLE_2;
    break;
  case (3):
    config |= ADS1X15_REG_CONFIG_MUX_SINGLE_3;
    break;
  }

  // Set 'start single-conversion' bit
  config |= ADS1X15_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  writeRegister(_ads1113_i2c_port, _i2cAddress, ADS1X15_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  ads1113_delay(_conversionDelay);

  // Read the conversion results
  // Shift 12-bit results right 4 bits for the ADS1X15
  return readRegister(_ads1113_i2c_port, _i2cAddress, ADS1X15_REG_POINTER_CONVERT) >> _bitShift;
}

int16_t ADS1115::readADC_Differential_0_1(uint16_t sampleRate) {
  // Start with default values
  uint16_t config =
      ADS1X15_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
      ADS1X15_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
      ADS1X15_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
      ADS1X15_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
      ADS1X15_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= _gain;

  // Set data rate
  config |= sampleRate;

  // Set channels
  config |= ADS1X15_REG_CONFIG_MUX_DIFF_0_1; // AIN0 = P, AIN1 = N

  // Set 'start single-conversion' bit
  config |= ADS1X15_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  writeRegister(_ads1113_i2c_port, _i2cAddress, ADS1X15_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  while (!conversionComplete())
    ;

  // Read the conversion results
  return getLastConversionResults();
}

void ADS1115::setGain(adsGain_t gain){
	_gain = gain;
	_multiplier = 1.0 / (MAX_VALUE >> _bitShift);
	switch (_gain) {
		case GAIN_TWOTHIRDS:
			_multiplier *= 6.144f;
			break;
		case GAIN_ONE:
			_multiplier *= 4.096f;
			break;
		case GAIN_TWO:
			_multiplier *= 2.048f;
			break;
		case GAIN_FOUR:
			_multiplier *= 1.024f;
			break;
		case GAIN_EIGHT:
			_multiplier *= 0.512f;
			break;
		case GAIN_SIXTEEN:
			_multiplier *= 0.256f;
			break;
		default:
			_multiplier *= 0.0f;
	}
}

adsGain_t ADS1115::getGain(){
	return _gain;
}
