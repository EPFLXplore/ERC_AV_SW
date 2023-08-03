#include "ADS1115.hpp"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "stdio.h"
#include "Build.h"

/*============================================================================*/
/* Redefine local functions													  */
/*============================================================================*/

void ads_delay(int time){
#ifdef FREERTOS_ENABLED
  osDelay(time);
#else
  HAL_Delay(time);
#endif
}

/*============================================================================*/
/* Local functions														      */
/*============================================================================*/

// Write the register
HAL_StatusTypeDef ADS1115::writeRegister(int8_t reg, uint16_t value) {
	uint8_t pData[2];
	pData[0] = (value >> 8);
	pData[1] = value & 0xff;
	HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(hi2c, m_i2cAddress, 10, 100);
	if(status != HAL_OK) {
		return status;
	}

	return HAL_I2C_Mem_Write(hi2c, m_i2cAddress, reg, I2C_MEMADD_SIZE_8BIT, pData, 2, 10);
}

// Read the register
HAL_StatusTypeDef ADS1115::readRegister(uint8_t reg, uint16_t& val) {
	HAL_StatusTypeDef status;
	uint8_t data[2] = {0, 0};

	status = HAL_I2C_Mem_Read(hi2c, m_i2cAddress, (uint16_t)reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)data, 2, 1000);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}
	val = (uint16_t)(data[0] << 8) | data[1];
	return HAL_OK;
}

// Check if we have correct connection.
HAL_StatusTypeDef ADS1115::ADSbegin() {
	HAL_StatusTypeDef status = HAL_I2C_Init(hi2c);

	if (status != HAL_OK)
		return status;

	return HAL_I2C_IsDeviceReady(hi2c, m_i2cAddress, 10, 100);
}


ADS1115::ADS1115(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress) {
	this->hi2c = hi2c;
	m_i2cAddress = i2cAddress << 1; //  It's Important to shift the address << 1
	m_conversionDelay = ADS1115_CONVERSIONDELAY;
	m_bitShift = 0; // 4 for ADS1115
	m_gain = GAIN_ONE;
	m_dataRate = ADS1115_REG_CONFIG_DR_128SPS;
	full_scale = 4.096f;
}

HAL_StatusTypeDef ADS1115::ADS1115_init() {
	//Deinit the port
	HAL_StatusTypeDef status = HAL_I2C_DeInit(hi2c);
	if (status != HAL_OK){
		return status;
	}

	status = ADSbegin();
	if (status != HAL_OK)
	{
		return status;
	}

	return HAL_OK;

}

/*
 * // The ADC input range (or gain) can be changed via the following
 // functions, but be careful never to exceed VDD +0.3V max, or to
 // exceed the upper and lower limits if you adjust the input range!
 // Setting these values incorrectly may destroy your ADC!

 // ADSsetGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 0.1875mV (default)
 // ADSsetGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 0.125mV
 // ADSsetGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 0.0625mV
 // ADSsetGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.03125mV
 // ADSsetGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.015625mV
 // ADSsetGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.0078125mV
 */
void ADS1115::ADSsetGain(adsGain_t gain) {
	m_gain = gain;
	switch(gain) {
	case GAIN_TWOTHIRDS:
		full_scale = 2*6.6144;
		break;
	case GAIN_ONE:
		full_scale = 2*4.096;
		break;
	case GAIN_TWO:
		full_scale = 2*2.048;
		break;
	case GAIN_FOUR:
		full_scale = 2*1.024;
		break;
	case GAIN_EIGHT:
		full_scale = 2*0.512;
		break;
	case GAIN_SIXTEEN:
		full_scale = 2*0.256;
		break;
	}
}

// Get the gain
adsGain_t ADS1115::ADSgetGain() {
	return m_gain;
}

// Gets a single-ended ADC reading from the specified channel
HAL_StatusTypeDef ADS1115::ADSreadADC_SingleEnded(uint8_t channel, int16_t& val) {
	if (channel > 3) {
		val = 0;
		return HAL_ERROR;
	}

	// Start with default values
	uint16_t config =
	ADS1115_REG_CONFIG_CQUE_NONE 			|   // Disable the comparator (default val)
			ADS1115_REG_CONFIG_CLAT_NONLAT 	|  	// Non-latching (default val)
			ADS1115_REG_CONFIG_CPOL_ACTVLOW | 	// Alert/Rdy active low   (default val)
			ADS1115_REG_CONFIG_CMODE_TRAD 	| 	// Traditional comparator (default val)
			ADS1115_REG_CONFIG_MODE_SINGLE;    	// Single-shot mode (default)

	// Set PGA/voltage range
	config |= m_gain;
	config |= m_dataRate;

	// Set single-ended input channel
	switch (channel) {
	case (0):
		config |= ADS1115_REG_CONFIG_MUX_SINGLE_0;
		break;
	case (1):
		config |= ADS1115_REG_CONFIG_MUX_SINGLE_1;
		break;
	case (2):
		config |= ADS1115_REG_CONFIG_MUX_SINGLE_2;
		break;
	case (3):
		config |= ADS1115_REG_CONFIG_MUX_SINGLE_3;
		break;
	}

	// Set 'start single-conversion' bit
	config |= ADS1115_REG_CONFIG_OS_SINGLE;

	// Write config register to the ADC
	HAL_StatusTypeDef status = writeRegister(ADS1115_REG_POINTER_CONFIG, config);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}
		// Wait for the conversion to complete
	ads_delay(m_conversionDelay);
//	while (!conversionComplete()) {}

	// Read the conversion results
	uint16_t res;
	status = readRegister(ADS1115_REG_POINTER_CONVERT, res);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}
	val = (int16_t)(res << m_bitShift);

	return HAL_OK;
}

/*
 * Reads the conversion results, measuring the voltage
 * difference between the P (AIN0) and N (AIN1) input.  Generates
 * a signed value since the difference can be either positive or negative.
 */
HAL_StatusTypeDef ADS1115::ADSreadADC_Differential_0_1(int16_t& val) {
	// Start with default vHAL_StatusTypeDefalues
	uint16_t config =
	ADS1115_REG_CONFIG_CQUE_NONE 	|   // Disable the comparator (default val)
	ADS1115_REG_CONFIG_CLAT_NONLAT 	|  	// Non-latching (default val)
	ADS1115_REG_CONFIG_CPOL_ACTVLOW | 	// Alert/Rdy active low   (default val)
	ADS1115_REG_CONFIG_CMODE_TRAD 	| 	// Traditional comparator (default val)
	ADS1115_REG_CONFIG_MODE_SINGLE;   	// Single-shot mode (default)

	// Set PGA/voltage range
	config |= m_gain;

	config |= m_dataRate;

	// Set channels
	config |= ADS1115_REG_CONFIG_MUX_DIFF_0_1; // AIN0 = P, AIN1 = N

	// Set 'start single-conversion' bit
	config |= ADS1115_REG_CONFIG_OS_SINGLE;

	// Write config register to the ADC
	HAL_StatusTypeDef status = writeRegister(ADS1115_REG_POINTER_CONFIG, config);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}

	// Wait for the conversion to complete
	ads_delay(m_conversionDelay);

	// Read the conversion results
	uint16_t res;
	status = readRegister(ADS1115_REG_POINTER_CONVERT, res);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}
	res = res >> m_bitShift;
	if (m_bitShift == 0) {
		val = (int16_t) res;
		return HAL_OK;
	} else {
		// Shift 12-bit results right 4 bits for the ADS1115,
		// making sure we keep the sign bit intact
		if (res > 0x07FF) {
			// negative number - extend the sign to 16th bit
			res |= 0xF000;
		}
		val = (int16_t) res;
		return HAL_OK;
	}
}

/*
 * Sets up the comparator to operate in basic mode, causing the
 * ALERT/RDY pin to assert (go from high to low) when the ADC
 * value exceeds the specified threshold.
 * This will also set the ADC in continuous conversion mode.
 */
HAL_StatusTypeDef ADS1115::ADSstartComparator_SingleEnded(uint8_t channel, int16_t threshold) {
	// Start with default values
	uint16_t config =
	ADS1115_REG_CONFIG_CQUE_1CONV 	|   	// Comparator enabled and asserts on 1 match
	ADS1115_REG_CONFIG_CLAT_LATCH 	|   	// Latching mode
	ADS1115_REG_CONFIG_CPOL_ACTVLOW | 	// Alert/Rdy active low   (default val)
	ADS1115_REG_CONFIG_CMODE_TRAD 	| 	// Traditional comparator (default val)
	ADS1115_REG_CONFIG_MODE_CONTIN;   	// Continuous conversion mode

	// Set PGA/voltage range
	config |= m_gain;

	config |= m_dataRate;

	// Set single-ended input channel
	switch (channel) {
	case (0):
		config |= ADS1115_REG_CONFIG_MUX_SINGLE_0;
		break;
	case (1):
		config |= ADS1115_REG_CONFIG_MUX_SINGLE_1;
		break;
	case (2):
		config |= ADS1115_REG_CONFIG_MUX_SINGLE_2;
		break;
	case (3):
		config |= ADS1115_REG_CONFIG_MUX_SINGLE_3;
		break;
	}

	HAL_StatusTypeDef status = writeRegister(ADS1115_REG_POINTER_HITHRESH, threshold << m_bitShift);
	if (status != HAL_OK)
		return status;
	// Write config register to the ADC
	return writeRegister(ADS1115_REG_POINTER_CONFIG, config);
}

/*
 * In order to clear the comparator, we need to read the conversion results.
 * This function reads the last conversion results without changing the config value.
 */
HAL_StatusTypeDef ADS1115::ADSgetLastConversionResults(int16_t& val) {
	// Wait for the conversion to complete
	ads_delay(m_conversionDelay);

	// Read the conversion results
	uint16_t res;
	HAL_StatusTypeDef status = readRegister(ADS1115_REG_POINTER_CONVERT, res);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}
	res = res >> m_bitShift;
	if (m_bitShift == 0) {
		val = (int16_t) res;
		return HAL_OK;
	} else {
		if (res > 0x07FF) {
			// negative number - extend the sign to 16th bit
			res |= 0xF000;
		}
		val = (int16_t) res;
		return HAL_OK;
	}
}

/*
 * Returns the current configured full scale (in V)
 */
float ADS1115::get_full_scale() {
	return full_scale;
}

HAL_StatusTypeDef ADS1115::read_average(uint8_t channel, float& val, uint16_t times) {
	long sum = 0;
	HAL_StatusTypeDef status;
	int16_t res = 0;
	for (uint16_t i = 0; i < times; i++) {
		status = ADSreadADC_SingleEnded(channel, res);
		if (status != HAL_OK) {
			val = 0;
			return status;
		}
		sum += res;
	}
	if(times==0) return HAL_ERROR;
	val = (float)sum / times;
	return HAL_OK;
}

HAL_StatusTypeDef ADS1115::get_value_offset(uint8_t channel, float& val, uint16_t times) {
	HAL_StatusTypeDef status = read_average(channel, val, times);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}
	val -= OFFSET[channel];
	return HAL_OK;
}

HAL_StatusTypeDef ADS1115::get_value_conv(uint8_t channel, float& val, uint16_t times) {
	HAL_StatusTypeDef status = get_value_offset(channel, val, times);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}
	val *= SCALE[channel];
	return HAL_OK;
}

void ADS1115::set_scale(uint8_t channel, float scale) {
	SCALE[channel] = scale;
}

float ADS1115::get_scale(uint8_t channel) {
	return SCALE[channel];
}

void ADS1115::set_offset(uint8_t channel, float offset) {
	OFFSET[channel] = offset;
}

float ADS1115::get_offset(uint8_t channel) {
	return OFFSET[channel];
}

bool ADS1115::conversionComplete() {
	uint16_t val;
	readRegister(ADS1115_REG_POINTER_CONFIG, val);
	return ((val & 0x8000) != 0);
}
