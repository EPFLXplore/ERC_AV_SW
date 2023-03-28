/*
 * ADS1015_ADS1115.h
 *
 *  Created on: Feb 9, 2020
 *      Author: Adafruit. Rewritten by Daniel Mårtensson
 */

#ifndef SRC_ADS1015_ADS1115_ADS1015_ADS1115_H_
#define SRC_ADS1015_ADS1115_ADS1015_ADS1115_H_

#include <stdbool.h>
#include <stdint.h>
#include "ADS1113_defines.h"
#include "stm32h7xx_hal.h"

class ADS1113 {
public:
	ADS1113(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress);
	bool ADS1113_init();
	bool ADSbegin();
	uint16_t ADSreadADC_SingleEnded(uint8_t channel);
	float ADSreadADC_Voltage();
	int16_t  ADSreadADC_Differential_0_1();
	void  ADSstartComparator_SingleEnded(uint8_t channel, int16_t threshold);
	int16_t  ADSgetLastConversionResults();
	void  ADSsetGain(adsGain_t gain);
	adsGain_t  ADSgetGain();
private:
	bool writeRegister(int8_t reg, uint16_t value);
	uint16_t readRegister(uint8_t reg);

	uint16_t m_i2cAddress;      ///< the I2C address
	uint32_t m_conversionDelay; ///< conversion delay
	uint8_t m_bitShift;        ///< bit shift amount
	float full_scale;
	adsGain_t m_gain;          ///< ADC gain
	I2C_HandleTypeDef* hi2c;    // Handle for I2C
};

#endif /* SRC_ADS1015_ADS1115_ADS1015_ADS1115_H_ */
