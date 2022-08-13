#include "ADS1113.h"

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
static bool writeRegister(ads1113_t *i2c, uint8_t reg, uint16_t value) {
//	uint8_t pData[3] = { reg, (uint8_t) (value >> 8), (uint8_t) (value & 0xFF) };
//	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->m_i2cAddress, pData, 3, 10);
	uint8_t pData[2];
	pData[0] = (value >> 8);
	pData[1] = value & 0xff;

	if(HAL_I2C_IsDeviceReady(i2c->hi2c, i2c->m_i2cAddress, 10, 10) != HAL_OK) {
		return false;
	}

	return HAL_I2C_Mem_Write(i2c->hi2c, i2c->m_i2cAddress, reg, I2C_MEMADD_SIZE_8BIT, pData, 2, 10) == HAL_OK;
}

// Read the register
static uint16_t readRegister(ads1113_t *i2c, uint8_t reg) {
	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->m_i2cAddress, &reg, 1, 10);
	uint8_t pData[2] = { 0, 0 };
	HAL_I2C_Master_Receive(i2c->hi2c, i2c->m_i2cAddress, pData, 2, 10);
	return ((pData[0] << 8) | pData[1]);
}

// Check if we have correct connection.
static bool ADSbegin(ads1113_t *i2c) {
//	if (HAL_I2C_IsDeviceReady(i2c->hi2c, i2c->m_i2cAddress, 10, 10) != HAL_OK)
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // This MUST have GPIO PA5 ready to use - ERROR I2C - Wrong address
	if (HAL_I2C_Init(i2c->hi2c) == HAL_OK){
		if(HAL_I2C_IsDeviceReady(i2c->hi2c, i2c->m_i2cAddress, 10, 100) == HAL_OK)
			return HAL_OK;
	}
	return HAL_ERROR;
}

// Declare an ADS1115 structure
bool ADS1113_init(ads1113_t *i2c, I2C_HandleTypeDef *hi2c, uint8_t i2cAddress) {
//	i2c->hi2c = hi2c;
//	i2c->m_i2cAddress = i2cAddress << 1; //  It's Important to shift the address << 1
//	i2c->m_conversionDelay = ADS1115_CONVERSIONDELAY;
//	i2c->m_bitShift = 0;
//	i2c->m_gain = GAIN_ONE; // +/- 4.096V: In our case VDD is 3.3V, so we must not exceed 3.6 V.
//	i2c->full_scale = 4.096f;
//	ADSbegin(i2c);

	i2c->hi2c = hi2c;
	i2c->m_i2cAddress = i2cAddress << 1; //  It's Important to shift the address << 1
	i2c->m_conversionDelay = ADS1115_CONVERSIONDELAY;
	i2c->m_bitShift = 0;
	i2c->m_gain = GAIN_ONE; // +/- 4.096V: In our case VDD is 3.3V, so we must not exceed 3.6 V.
	i2c->full_scale = 4.096f;

	//Deinit the port
	if (HAL_I2C_DeInit(i2c->hi2c) != HAL_OK){
		return false;
	}
	//Configure I2C port
	i2c->hi2c->Instance = I2C1;
	i2c->hi2c->Init.Timing = 0x00401242;
	i2c->hi2c->Init.OwnAddress1 = 0;
	i2c->hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	i2c->hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2c->hi2c->Init.OwnAddress2 = 0;
	i2c->hi2c->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	i2c->hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2c->hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(i2c->hi2c) != HAL_OK)
	{
		return false;
	}
	/** Configure Analogue filter
	*/
	if (HAL_I2CEx_ConfigAnalogFilter(i2c->hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		return false;
	}
	/** Configure Digital filter
	*/
	if (HAL_I2CEx_ConfigDigitalFilter(i2c->hi2c, 0x0F) != HAL_OK)
	{
		return false;
	}
	/** I2C Enable Fast Mode Plus
	*/
	HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);

	if (ADSbegin(i2c) != HAL_OK)
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
void ADSsetGain(ads1113_t *i2c, adsGain_t gain) {
	i2c->m_gain = gain;
	switch(gain) {
	case GAIN_TWOTHIRDS:
		i2c->full_scale = 6.6144;
		break;
	case GAIN_ONE:
		i2c->full_scale = 4.096;
		break;
	case GAIN_TWO:
		i2c->full_scale = 2.048;
		break;
	case GAIN_FOUR:
		i2c->full_scale = 1.024;
		break;
	case GAIN_EIGHT:
		i2c->full_scale = 0.512;
		break;
	case GAIN_SIXTEEN:
		i2c->full_scale = 0.256;
		break;
	}
}

// Get the gain
adsGain_t ADSgetGain(ads1113_t *i2c) {
	return i2c->m_gain;
}

// Gets a single-ended ADC reading from the specified channel
uint16_t ADSreadADC_SingleEnded(ads1113_t *i2c, uint8_t channel) {
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
	config |= i2c->m_gain;

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
	writeRegister(i2c, ADS1015_REG_POINTER_CONFIG, config);

	// Wait for the conversion to complete
	ads_delay(i2c->m_conversionDelay);

	// Read the conversion results
	// Shift 12-bit results right 4 bits for the ADS1015
	return readRegister(i2c, ADS1015_REG_POINTER_CONVERT) >> i2c->m_bitShift;
}

/*
 * Reads the conversion results, measuring the voltage
 * difference between the P (AIN0) and N (AIN1) input.  Generates
 * a signed value since the difference can be either positive or negative.
 */
int16_t ADSreadADC_Differential_0_1(ads1113_t *i2c) {
	// Start with default values
	uint16_t config =
	ADS1015_REG_CONFIG_CQUE_NONE 	|   // Disable the comparator (default val)
	ADS1015_REG_CONFIG_CLAT_NONLAT 	|  	// Non-latching (default val)
	ADS1015_REG_CONFIG_CPOL_ACTVLOW | 	// Alert/Rdy active low   (default val)
	ADS1015_REG_CONFIG_CMODE_TRAD 	| 	// Traditional comparator (default val)
	ADS1015_REG_CONFIG_DR_1600SPS 	| 	// 1600 samples per second (default)
	ADS1015_REG_CONFIG_MODE_SINGLE;   	// Single-shot mode (default)

	// Set PGA/voltage range
	config |= i2c->m_gain;

	// Set channels
	config |= ADS1015_REG_CONFIG_MUX_DIFF_0_1; // AIN0 = P, AIN1 = N

	// Set 'start single-conversion' bit
	config |= ADS1015_REG_CONFIG_OS_SINGLE;

	// Write config register to the ADC
	writeRegister(i2c, ADS1015_REG_POINTER_CONFIG, config);

	// Wait for the conversion to complete
	ads_delay(i2c->m_conversionDelay);

	// Read the conversion results
	uint16_t res = readRegister(i2c, ADS1015_REG_POINTER_CONVERT) >> i2c->m_bitShift;
	if (i2c->m_bitShift == 0) {
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


float ADSreadADC_Voltage(ads1113_t *i2c) {
//	float coeff = i2c->full_scale/(ADS_MAX_VALUE*ADS_VOLTAGE_DIVIDER_RATIO);
//	return ADSreadADC_Differential_0_1(i2c) * coeff;
	float coeff = i2c->full_scale*ADS_VOLTAGE_DIVIDER_RATIO/ADS_MAX_VALUE;
	return ADSreadADC_Differential_0_1(i2c) * ADS_COEFF + ADS_OFFSET;
}

/*
 * Sets up the comparator to operate in basic mode, causing the
 * ALERT/RDY pin to assert (go from high to low) when the ADC
 * value exceeds the specified threshold.
 * This will also set the ADC in continuous conversion mode.
 */
void ADSstartComparator_SingleEnded(ads1113_t *i2c, uint8_t channel, int16_t threshold) {
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
	config |= i2c->m_gain;

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
	writeRegister(i2c, ADS1015_REG_POINTER_HITHRESH, threshold << i2c->m_bitShift);

	// Write config register to the ADC
	writeRegister(i2c, ADS1015_REG_POINTER_CONFIG, config);
}

/*
 * In order to clear the comparator, we need to read the conversion results.
 * This function reads the last conversion results without changing the config value.
 */
int16_t ADSgetLastConversionResults(ads1113_t *i2c) {
	// Wait for the conversion to complete
	HAL_Delay(i2c->m_conversionDelay);

	// Read the conversion results
	uint16_t res = readRegister(i2c, ADS1015_REG_POINTER_CONVERT) >> i2c->m_bitShift;
	if (i2c->m_bitShift == 0) {
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
