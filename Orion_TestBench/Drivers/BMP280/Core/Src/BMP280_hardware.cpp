#include "main.h"
#include "BMP280.hpp"
#include "BMP280_defines.h"
#include "stm32h7xx_hal.h"
#include "cmsis_os.h"

#include <stdint.h>

void bmp_delay(uint32_t time){
#ifdef FREERTOS_ENABLED
  osDelay(time);
#else
  HAL_Delay(time);
#endif
}


/* ---------------------------------------------------------------------------
 * Register read/write definitions -------------------------------------------
 * ------------------------------------------------------------------------ */

/**
 * Read a register
 * @param address: Register address.
 * @return Register value.
 * */
uint8_t BMP280::readRegister(uint8_t address)
{
	spiCSNlow();
	spiReadWrite(address);
	uint8_t value = spiReadWrite(0);
	spiCSNhigh();
	return value;
}

/**
 * Write to a register
 * @param address: Register address.
 * @param value: Value to write.
 * */
void BMP280::writeRegister(uint8_t address, uint8_t value)
{
	spiCSNlow();
	spiReadWrite(address & BMP280_SPI_MASK_WRITE);
	spiReadWrite(value);
	spiCSNhigh();
}

/**
 * Read a multi-byte register
 * @param address: Register address.
 * @param values: Array pointer to store values in.
 * @param length: Number of bytes to read.
 * */
void BMP280::readMBRegister(uint8_t address, uint8_t *values, uint8_t length)
{
	spiCSNlow();
	spiReadWrite(address);
	while (length--)
	{
		*values++ = spiReadWrite(0);
	}
	spiCSNhigh();
}


/* ---------------------------------------------------------------------------
 * SPI interface definitions -------------------------------------------------
 * (ADAPT THESE METHODS TO YOUR HARDWARE) ------------------------------------
 * ------------------------------------------------------------------------ */

/**
 * SPI transmit and receive one byte simultaneously
 * @param tx_message: Transmit byte.
 * @return Received byte.
 * */
uint8_t BMP280::spiReadWrite(uint8_t tx_message)
{
	uint8_t rx_message = 255;
	HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(this->spiHandle, &tx_message, &rx_message, 1,
			HAL_MAX_DELAY);
	return rx_message;
}

/** Pull chip select high (inactive) */
void BMP280::spiCSNhigh()
{
	HAL_GPIO_WritePin(BMP280_CSN_GPIO_Port, BMP280_CSN_Pin, GPIO_PIN_SET);
}

/** Pull chip select low (active) */
void BMP280::spiCSNlow()
{
	HAL_GPIO_WritePin(BMP280_CSN_GPIO_Port, BMP280_CSN_Pin, GPIO_PIN_RESET);
}

/** Millisecond Delay */
void BMP280::delay_ms(uint32_t milliseconds)
{
	bmp_delay(milliseconds);
}
