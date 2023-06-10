#include <ADS1113.hpp>
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
bool ADS1113::writeRegister(int8_t reg, uint16_t value) {
	HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(hi2c, m_i2cAddress, 10, 100);
	uint8_t pData[2];
	pData[0] = (value >> 8);
	pData[1] = value & 0xff;
	status = HAL_I2C_IsDeviceReady(hi2c, m_i2cAddress, 10, 100);
	if(status != HAL_OK) {
		return false;
	}

	return HAL_I2C_Mem_Write(hi2c, m_i2cAddress, reg, I2C_MEMADD_SIZE_8BIT, pData, 2, 10) == HAL_OK;
}

// Read the register
uint16_t ADS1113::readRegister(uint8_t reg) {
	HAL_I2C_Master_Transmit(hi2c, m_i2cAddress, &reg, 1, 10);
	uint8_t pData[2] = { 0, 0 };
	HAL_StatusTypeDef status = HAL_I2C_Master_Receive(hi2c, m_i2cAddress, pData, 2, 10);
	if (status == HAL_OK) {
		return ((pData[0] << 8) | pData[1]);
	} else {
		printf("[ADS1113] Cannot read register.");
		return 0;
	}
}

// Check if we have correct connection.
bool ADS1113::ADSbegin() {
	if (HAL_I2C_Init(hi2c) == HAL_OK){
		HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(hi2c, m_i2cAddress, 10, 100);
		if(status == HAL_OK)
			return HAL_OK;
	}
	return HAL_ERROR;
}


ADS1113::ADS1113(I2C_HandleTypeDef* hi2c, uint8_t i2cAddress) {
	this->hi2c = hi2c;
	m_i2cAddress = i2cAddress << 1; //  It's Important to shift the address << 1
	m_conversionDelay = ADS1115_CONVERSIONDELAY;
	m_bitShift = 0;
	m_gain = GAIN_ONE;
	full_scale = 4.096f;
}

bool ADS1113::ADS1113_init() {
	//Deinit the port
	if (HAL_I2C_DeInit(hi2c) != HAL_OK){
		return false;
	}

	hi2c->Init.Timing = 0x00401242;
	hi2c->Init.OwnAddress1 = 0;
	hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c->Init.OwnAddress2 = 0;
	hi2c->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(hi2c) != HAL_OK)
	{
		return false;
	}
	/** Configure Analogue filter
	*/
	if (HAL_I2CEx_ConfigAnalogFilter(hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		return false;
	}
	/** Configure Digital filter
	*/
	if (HAL_I2CEx_ConfigDigitalFilter(hi2c, 0x0F) != HAL_OK)
	{
		return false;
	}
	/** I2C Enable Fast Mode Plus
	*/
	if (hi2c->Instance = I2C1)
		HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
	else if (hi2c->Instance = I2C2)
		HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C2);
	else if (hi2c->Instance = I2C3)
		HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C3);


	if (ADSbegin() != HAL_OK)
	{
		return false;
	}

	return true;

}

/*
 * // The ADC input range (or gain) can be changed via the following
 // functions, but be careful never to exceed VDD +0.3V max, or to
 // exceed the upper and lower limits if you adjust the input range!
 // Setting these values incorrectly may destroy your ADC!
 //                                                                ADS1015  ADS1115
 //                                                                -------  -------
 // ADSsetGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
 // ADSsetGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
 // ADSsetGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
 // ADSsetGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
 // ADSsetGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
 // ADSsetGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
 */
void ADS1113::ADSsetGain(adsGain_t gain) {
	m_gain = gain;
	switch(gain) {
	case GAIN_TWOTHIRDS:
		full_scale = 6.6144;
		break;
	case GAIN_ONE:
		full_scale = 4.096;
		break;
	case GAIN_TWO:
		full_scale = 2.048;
		break;
	case GAIN_FOUR:
		full_scale = 1.024;
		break;
	case GAIN_EIGHT:
		full_scale = 0.512;
		break;
	case GAIN_SIXTEEN:
		full_scale = 0.256;
		break;
	}
}

// Get the gain
adsGain_t ADS1113::ADSgetGain() {
	return m_gain;
}

// Gets a single-ended ADC reading from the specified channel
uint16_t ADS1113::ADSreadADC_SingleEnded(uint8_t channel) {
	if (channel > 3) {
		return 0;
	}

	// Start with default values
	uint16_t config =
	ADS1015_REG_CONFIG_CQUE_NONE 			|   	// Disable the comparator (default val)
			ADS1015_REG_CONFIG_CLAT_NONLAT 	|  	// Non-latching (default val)
			ADS1015_REG_CONFIG_CPOL_ACTVLOW | 	// Alert/Rdy active low   (default val)
			ADS1015_REG_CONFIG_CMODE_TRAD 	| 	// Traditional comparator (default val)
			ADS1015_REG_CONFIG_DR_1600SPS 	| 	// 1600 samples per second (default)
			ADS1015_REG_CONFIG_MODE_SINGLE;   	// Single-shot mode (default)

	// Set PGA/voltage range
	config |= m_gain;

	// Set single-ended input channel
	switch (channel) {
	case (0):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
		break;
	case (1):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
		break;
	}

	// Set 'start single-conversion' bit
	config |= ADS1015_REG_CONFIG_OS_SINGLE;

	// Write config register to the ADC
	writeRegister(ADS1015_REG_POINTER_CONFIG, config);

	// Wait for the conversion to complete
	ads_delay(m_conversionDelay);

	// Read the conversion results
	// Shift 12-bit results right 4 bits for the ADS1015
	return readRegister(ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
}

/*
 * Reads the conversion results, measuring the voltage
 * difference between the P (AIN0) and N (AIN1) input.  Generates
 * a signed value since the difference can be either positive or negative.
 */
int16_t ADS1113::ADSreadADC_Differential_0_1() {
	// Start with default values
	uint16_t config =
	ADS1015_REG_CONFIG_CQUE_NONE 	|   // Disable the comparator (default val)
	ADS1015_REG_CONFIG_CLAT_NONLAT 	|  	// Non-latching (default val)
	ADS1015_REG_CONFIG_CPOL_ACTVLOW | 	// Alert/Rdy active low   (default val)
	ADS1015_REG_CONFIG_CMODE_TRAD 	| 	// Traditional comparator (default val)
	ADS1015_REG_CONFIG_DR_1600SPS 	| 	// 1600 samples per second (default)
	ADS1015_REG_CONFIG_MODE_SINGLE;   	// Single-shot mode (default)

	// Set PGA/voltage range
	config |= m_gain;

	// Set channels
	config |= ADS1015_REG_CONFIG_MUX_DIFF_0_1; // AIN0 = P, AIN1 = N

	// Set 'start single-conversion' bit
	config |= ADS1015_REG_CONFIG_OS_SINGLE;

	// Write config register to the ADC
	bool success = writeRegister(ADS1015_REG_POINTER_CONFIG, config);

	// Wait for the conversion to complete
	ads_delay(m_conversionDelay);

	// Read the conversion results
	uint16_t res = readRegister(ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
	if (m_bitShift == 0) {
		return (int16_t) res;
	} else {
		// Shift 12-bit results right 4 bits for the ADS1015,
		// making sure we keep the sign bit intact
		if (res > 0x07FF) {
			// negative number - extend the sign to 16th bit
			res |= 0xF000;
		}
		return (int16_t) res;
	}
}


float ADS1113::ADSreadADC_Voltage() {
	float coeff = full_scale*ADS_VOLTAGE_DIVIDER_RATIO/ADS_MAX_VALUE;
	return ADSreadADC_Differential_0_1() * ADS_COEFF + ADS_OFFSET;
}

/*
 * Sets up the comparator to operate in basic mode, causing the
 * ALERT/RDY pin to assert (go from high to low) when the ADC
 * value exceeds the specified threshold.
 * This will also set the ADC in continuous conversion mode.
 */
void ADS1113::ADSstartComparator_SingleEnded(uint8_t channel, int16_t threshold) {
	// Start with default values
	uint16_t config =
	ADS1015_REG_CONFIG_CQUE_1CONV 	|   	// Comparator enabled and asserts on 1 match
	ADS1015_REG_CONFIG_CLAT_LATCH 	|   	// Latching mode
	ADS1015_REG_CONFIG_CPOL_ACTVLOW | 	// Alert/Rdy active low   (default val)
	ADS1015_REG_CONFIG_CMODE_TRAD 	| 	// Traditional comparator (default val)
	ADS1015_REG_CONFIG_DR_1600SPS 	|	 // 1600 samples per second (default)
	ADS1015_REG_CONFIG_MODE_CONTIN 	|  	// Continuous conversion mode
	ADS1015_REG_CONFIG_MODE_CONTIN;   	// Continuous conversion mode

	// Set PGA/voltage range
	config |= m_gain;

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
	writeRegister(ADS1015_REG_POINTER_HITHRESH, threshold << m_bitShift);

	// Write config register to the ADC
	writeRegister(ADS1015_REG_POINTER_CONFIG, config);
}

/*
 * In order to clear the comparator, we need to read the conversion results.
 * This function reads the last conversion results without changing the config value.
 */
int16_t ADS1113::ADSgetLastConversionResults() {
	// Wait for the conversion to complete
	HAL_Delay(m_conversionDelay);

	// Read the conversion results
	uint16_t res = readRegister(ADS1015_REG_POINTER_CONVERT) >> m_bitShift;
	if (m_bitShift == 0) {
		return (int16_t) res;
	} else {
		// Shift 12-bit results right 4 bits for the ADS1015,
		// making sure we keep the sign bit intact
		if (res > 0x07FF) {
			// negative number - extend the sign to 16th bit
			res |= 0xF000;
		}
		return (int16_t) res;
	}
}
