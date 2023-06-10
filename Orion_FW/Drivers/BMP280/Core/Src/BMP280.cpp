#include "BMP280.hpp"
#include "BMP280_defines.h"
#include "stm32h7xx_hal.h"

#include <math.h>
#include <stdint.h>


BMP280::BMP280(SPI_HandleTypeDef *handle)
{
	spiHandle = handle;
}

/** Initialize the device with desired configuration
 * @return 1, if device is not recognized, 0 otherwise.
 * */
uint8_t BMP280::initialize()
{
	uint8_t id = getID();
	if (id != BMP280_CHIP_ID)
	{
		return 1;
	}

	// Reset device and wait
	reset();
	delay_ms(500);

	// BEGIN OF CONFIGURATION ----------------------------------
	setPressureOversampling(oversampling_x16);
	setTemperatureOversampling(oversampling_x2);

	setPowerMode(mode_normal);
	setFilterCoefficient(filter_coeff_16);
	setStandbyTime(standby_time_500us);
	// END OF CONFIGURATION --------------------------

	readCompensationParameters();
	setReferencePressure(100, 50);

	return 0;
}

/** Perform power-on reset procedure */
void BMP280::reset()
{
	writeRegister(BMP280_REG_RESET, BMP280_RESET_VALUE);
}

/**
 * Read chip identification number.
 * @return chip ID
 * */
uint8_t BMP280::getID()
{
	return readRegister(BMP280_REG_ID);
}

/** Read calibration data from non-volatile sensor registers */
void BMP280::readCompensationParameters()
{
	uint8_t buf[24];
	readMBRegister(BMP280_REG_CALIB, buf, 24);
	compensationParameters.dig_t1 = ((buf[1] << 8) | buf[0]);
	compensationParameters.dig_t2 = ((buf[3] << 8) | buf[2]);
	compensationParameters.dig_t3 = ((buf[5] << 8) | buf[4]);
	compensationParameters.dig_p1 = ((buf[7] << 8) | buf[6]);
	compensationParameters.dig_p2 = ((buf[9] << 8) | buf[8]);
	compensationParameters.dig_p3 = ((buf[11] << 8) | buf[10]);
	compensationParameters.dig_p4 = ((buf[13] << 8) | buf[12]);
	compensationParameters.dig_p5 = ((buf[15] << 8) | buf[14]);
	compensationParameters.dig_p6 = ((buf[17] << 8) | buf[16]);
	compensationParameters.dig_p7 = ((buf[19] << 8) | buf[18]);
	compensationParameters.dig_p8 = ((buf[21] << 8) | buf[20]);
	compensationParameters.dig_p9 = ((buf[23] << 8) | buf[22]);
}

/**
 * Set reference pressure for altitude calculation by averaging pressure measurements.
 * @param samples: Number of measurements to average.
 * @param delay: Delay between measurements (in ms).
 * */
void BMP280::setReferencePressure(uint16_t samples, uint8_t delay)
{
	delay_ms(500);
	float sum = 0;
	for (char i = 0; i < samples; i++)
	{
		measure();
		sum += measurement.pressure;
		delay_ms(delay);
	}
	p_reference = sum / samples;

}

/** Configure pressure oversampling */
void BMP280::setPressureOversampling(Oversampling osrs_p)
{
	uint8_t ctrl = readRegister(BMP280_REG_CTRL_MEAS);
	ctrl = (ctrl & 0b11100011) | (osrs_p << 2);
	writeRegister(BMP280_REG_CTRL, ctrl);
}

/** Configure temperature oversampling */
void BMP280::setTemperatureOversampling(Oversampling osrs_t)
{
	uint8_t ctrl = readRegister(BMP280_REG_CTRL_MEAS);
	ctrl = (ctrl & 0b00011111) | (osrs_t << 5);
	writeRegister(BMP280_REG_CTRL, ctrl);
}

/** Configure power mode */
void BMP280::setPowerMode(PowerMode mode)
{
	uint8_t ctrl = readRegister(BMP280_REG_CTRL_MEAS);
	ctrl = (ctrl & 0b11111100) | mode;
	writeRegister(BMP280_REG_CTRL, ctrl);
}

/** Configure standby time */
void BMP280::setStandbyTime(StandbyTime t_sb)
{
	uint8_t conf = readRegister(BMP280_REG_CONFIG);
	conf = (conf & 0b00011111) | (t_sb << 5);
	writeRegister(BMP280_REG_CONFIG, conf);
}

/** Configure IIR filter */
void BMP280::setFilterCoefficient(FilterSetting filter)
{
	uint8_t conf = readRegister(BMP280_REG_CONFIG);
	conf = (conf & 0b11100011) | (filter << 2);
	writeRegister(BMP280_REG_CONFIG, conf);
}

/* ---------------------------------------------------------------------------
 * Measurements and compensation ---------------------------------------------
 * ------------------------------------------------------------------------ */

/**
 * Read latest measurement from sensor and execute compensation.
 * Stores the results in measurement member variable.
 * */
void BMP280::measure()
{
	uint8_t data[6];
	readMBRegister(BMP280_REG_DATA, data, 6);

	int32_t adc_P = data[0] << 12 | data[1] << 4 | data[2] >> 4;
	int32_t adc_T = data[3] << 12 | data[4] << 4 | data[5] >> 4;

	measurement.temperature = (float) compensate_temperature(adc_T) / 100.0;
	measurement.pressure = (float) compensate_pressure(adc_P) / 256.0;

	if (p_reference > 0)
	{
		measurement.altitude = (1.0
				- pow(measurement.pressure / p_reference, 0.1903)) * 4433076.0;
	}
}
