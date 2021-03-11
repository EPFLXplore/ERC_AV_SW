/*
 * ADS1113.cpp
 *
 *  Created on: Mar 7, 2021
 *      Author: AV Team
 */

#include <Libraries/ADS1113/ads1113.h>

void ADS1113::ads1113_delay(int time){
#ifdef FREERTOS_ENABLED
  osDelay(time);
#else
  HAL_Delay(time);
#endif
}

ADS1113::ADS1113(I2C_HandleTypeDef *hi2c, uint8_t i2cAddress):
	_i2cAddress(i2cAddress),
	_conversionDelay(ADS1113_CONVERSIONDELAY),
	_bitShift(0),
	_ads1113_i2c_port(hi2c)
{}

bool ADS1113::begin(){
	_ads1113_i2c_port->Init.Timing = 0x00602173;
	_ads1113_i2c_port->Init.OwnAddress1 = 0;
	_ads1113_i2c_port->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	_ads1113_i2c_port->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	_ads1113_i2c_port->Init.OwnAddress2 = 0;
	_ads1113_i2c_port->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	_ads1113_i2c_port->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	_ads1113_i2c_port->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(_ads1113_i2c_port) != HAL_OK){
		return false;
	}
	return true;
}

/* Write to register in ADS1113 directly */
static void writeRegister(I2C_HandleTypeDef* i2c_port, uint16_t i2cAddress, uint8_t reg, uint16_t value) {
	uint8_t pData[2];
	pData[0]=value & 0xff;
	pData[1]=(value >> 8);
	i2cAddress << 8;

	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(i2c_port, i2cAddress, pData, sizeof(value), 10);
	if (status == HAL_OK) {
		return;
	}
}

/* Read from register in ADS1113 directly */
static uint16_t readRegister(I2C_HandleTypeDef* i2c_port, uint16_t i2cAddress, uint8_t reg) {
	i2cAddress << 8;
	uint8_t pData[2];
	HAL_I2C_Master_Receive(i2c_port, i2cAddress, pData, sizeof(uint16_t), 10);
	return ((pData[0] << 8) | pData[1]); //CHECK HERE IF THERE ARE ERRORS : inverse pData[0] and pData[1]
}

uint16_t ADS1113::readADC_SingleEnded(uint16_t sampleRate) {
  // Start with default values
  uint16_t config =
      ADS1015_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
      ADS1015_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
      ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
      ADS1015_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
      sampleRate 					|   // 1600 samples per second (default)
      ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set 'start single-conversion' bit
  config |= ADS1015_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  writeRegister(_ads1113_i2c_port, _i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  ads1113_delay(_conversionDelay);

  // Read the conversion results
  // Shift 12-bit results right 4 bits for the ADS1015
  return readRegister(_ads1113_i2c_port, _i2cAddress, ADS1015_REG_POINTER_CONVERT) >> _bitShift;
}

int16_t ADS1113::getLastConversionResults() {
	// Wait for the conversion to complete
	ads1113_delay(_conversionDelay);

	// Read the conversion results
	uint16_t res = readRegister(_ads1113_i2c_port, _i2cAddress, ADS1015_REG_POINTER_CONVERT) >> _bitShift;
	if (_bitShift == 0) {
		return (int16_t)res;
	}
	else {
	// Shift 12-bit results right 4 bits for the ADS1015,
	// making sure we keep the sign bit intact
		if (res > 0x07FF) {
			// negative number - extend the sign to 16th bit
			res |= 0xF000;
		}
	return (int16_t)res;
	}
}

ADS1115::ADS1115(I2C_HandleTypeDef *hi2c, uint8_t i2cAddress):
	ADS1113(hi2c, i2cAddress),
	_gain(GAIN_TWOTHIRDS) /* +/- 6.144V range (limited to VDD +0.3V max!) */
{}

void ADS1115::startComparator_SingleEnded(uint8_t channel, int16_t threshold, uint16_t sampleRate) {
  // Start with default values
  uint16_t config =
      ADS1015_REG_CONFIG_CQUE_1CONV |   // Comparator enabled and asserts on 1
                                        // match
      ADS1015_REG_CONFIG_CLAT_LATCH |   // Latching mode
      ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
      ADS1015_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
      sampleRate					 |   // 1600 samples per second (default)
      ADS1015_REG_CONFIG_MODE_CONTIN |  // Continuous conversion mode
      ADS1015_REG_CONFIG_MODE_CONTIN;   // Continuous conversion mode

  // Set PGA/voltage range
  config |= _gain;

  // Set single-ended input channel
  switch (channel) {
  case (0):
    config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
    break;
  case (1):
    config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
    break;
  case (2):
    config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
    break;
  case (3):
    config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
    break;
  }

  // Set the high threshold register
  // Shift 12-bit results left 4 bits for the ADS1015
  writeRegister(_ads1113_i2c_port, _i2cAddress, ADS1015_REG_POINTER_HITHRESH, threshold << _bitShift);

  // Write config register to the ADC
  writeRegister(_ads1113_i2c_port, _i2cAddress, ADS1015_REG_POINTER_CONFIG, config);
}

uint16_t ADS1115::readADC_SingleEnded(uint8_t channel, uint16_t sampleRate) {
  if (channel > 3) {
    return 0;
  }

  // Start with default values
  uint16_t config =
      ADS1015_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
      ADS1015_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
      ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
      ADS1015_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
      sampleRate |   // 1600 samples per second (default)
      ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= _gain;

  // Set single-ended input channel
  switch (channel) {
  case (0):
    config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
    break;
  case (1):
    config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
    break;
  case (2):
    config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
    break;
  case (3):
    config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
    break;
  }

  // Set 'start single-conversion' bit
  config |= ADS1015_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  writeRegister(_ads1113_i2c_port, _i2cAddress, ADS1015_REG_POINTER_CONFIG, config);

  // Wait for the conversion to complete
  ads1113_delay(_conversionDelay);

  // Read the conversion results
  // Shift 12-bit results right 4 bits for the ADS1015
  return readRegister(_ads1113_i2c_port, _i2cAddress, ADS1015_REG_POINTER_CONVERT) >> _bitShift;
}

void ADS1115::setGain(adsGain_t gain){
	_gain = gain;
}

adsGain_t ADS1115::getGain(){
	return _gain;
}
