
/**
 * @note       STILL IN DEVELOPMENT
 * @file       max11615.c
 * @author     Simon Burkhardt github.com/mnemocron
 * @copyright  MIT license
 * @date       06.2018
 * @brief      C library for the MAX11615EEE+ I2C ADC for STM32 HAL.
 * @details
 * @see        github.com/mnemocron
 * @see        https://datasheets.maximintegrated.com/en/ds/MAX11612-MAX11617.pdf
 * @see        https://github.com/AllAboutEE/MAX11609EEE-Breakout-Board/tree/master/Software/Arduino/AllAboutEE-MAX11609-Library
 */
#include "max11615.h"
#include "main.h"

/**
 * @brief      writes a single value into a MAX11615 register
 * @param      *chip, pointer to the MAX11615 typedef struct
 * @param      reg, the destination register's address
 * @param      val, the value for the destination register
 */
uint8_t MAX11615_Write8(MAX11615 *chip, uint8_t reg, uint8_t val){
 	if(HAL_I2C_Mem_Write(chip->wireIface, chip->devAddress, reg, 1, &val, 1, 10) != HAL_OK)
		return 1;
	return 0;
}

/**
 * @brief       reads a single value from a MAX11615 register
 * @param       *chip, pointer to the MAX11615 typedef struct
 * @param       reg, the destination register's address
 * @param       val, pointer to the location where the value shall be stored
 * @return      0 on success, 1 on transmission error
 */
uint8_t MAX11615_Read8(MAX11615 *chip, uint8_t reg, uint8_t *val){
	if(HAL_I2C_Mem_Read(chip->wireIface, chip->devAddress, reg, 1, val, 1, 10) != HAL_OK)
		return 1;
	return 0;
}

/**
 * @brief        sets up the MAX11615 chip with the provided Vref setting
 * @param        *chip, pointer to the MAX11615 typedef struct
 * @param        *wireIface a pointer to a HAL I2C_HandleTypeDef
 * @param 		 address of the chip on the I2C bus
 */
uint8_t MAX11615_Init(MAX11615* chip, I2C_HandleTypeDef* wireIface, uint16_t address, uint8_t vRef){
	chip->wireIface = wireIface;
	chip->devAddress = address;
	uint8_t ret = 0;
	// 0 - don't care
	// 1 - reset configuration register to default
	// 2 - unipolar
	// 3 - internal clock
	// 4 - SEL0 (vRef)
	// 5 - SEL1 (vRef)
	// 6 - SEL2 (vRef)
	uint8_t setup_byte = 0xa6;
	ret += MAX11615_Setup(chip, vRef);
	// 0 - Single Ended
	// 1 to 4 - Channel Select:  7
	// 5 to 6 - Scan Mode: read channels up to selected channel
	uint8_t config_byte = 0x00;
	ret += MAX11615_Configuration(chip, config_byte);
	return ret;
}

uint8_t MAX11615_Setup(MAX11615* chip, uint8_t data){
	data = data | 0x80; // make REG bit 7 = 1 (setup byte)
	if(HAL_I2C_Master_Transmit(chip->wireIface, chip->devAddress, &data, 1, 10) != HAL_OK)
		return 1;
	return 0;
}

uint8_t MAX11615_Configuration(MAX11615* chip, uint8_t data){
	data = data & (~0x80); // make REG bit 7 = 0 (configuration byte)
	if(HAL_I2C_Master_Transmit(chip->wireIface, chip->devAddress, &data, 1, 10) != HAL_OK)
		return 1;
	return 0;
}

/**
 * Reads one channel.
 * @author Miguel  (5/24/2015)
 * @param channel  The channel to convert or read. Alternatively
 *                 if a channel was set already leave null.
 * @param val      Pointer to where the return value should be stored.
 * @return uint8_t
 */
uint8_t MAX11615_ADC_Read(MAX11615* chip, uint8_t channel, uint16_t* val){
	uint8_t result[2] = {0,0};

	// the conversion consists of two bytes per channel
	if(HAL_I2C_Master_Receive(chip->wireIface, chip->devAddress, &result[0], 2, HAL_MAX_DELAY) != HAL_OK){
		return 1;
	}
	uint16_t value = 0;
	// cast to uint16_t is necessary to not loose the values by the left shift
	value =  (((uint16_t)result[0] & 0x000f) << 8); // MSB is returned first
	value += ((uint16_t)result[1] & 0x00ff); // read LSB
	*val = value;
	return 0;
}

/**
 * Reads all channels conversion into a buffer/array.
 *
 * @author Miguel (5/24/2015)
 *
 * @param buffer an array where the channel read values are put.
 */
uint8_t MAX11615_Scan(MAX11615* chip, uint16_t* buffer){
	uint8_t ret = 0;
	uint8_t configurationByte = 0xf0;
	ret += MAX11615_Configuration(chip, configurationByte);
	// 2 bytes per channel. There are 8 channels
	for(uint8_t i = 0;i<8;i++){
		ret += MAX11615_ADC_Read(chip, i, buffer+i);
	}
	return ret;
}
