/*
 * MAX11615_sens.h
 *
 *  Created on: May 11, 2023
 *      Author: leo
 */

#ifndef MAX11615_CORE_INC_MAX11615_SENS_H_
#define MAX11615_CORE_INC_MAX11615_SENS_H_

#include "stm32h7xx_hal.h"



class Max11615_Sens{
public:
	Max11615_Sens(I2C_HandleTypeDef i2c, uint16_t dev_addr): i2c(i2c), dev_addr(dev_addr), channel(0), reference_v(5){};
	Max11615_Sens(I2C_HandleTypeDef i2c, uint16_t dev_addr, uint8_t channel, uint8_t reference_v):
		i2c(i2c), dev_addr(dev_addr), channel(channel), reference_v(reference_v){};

	bool init();


private:
	uint16_t dev_addr;
	I2C_HandleTypeDef i2c;
	bool write_reg(uint8_t reg, uint8_t val);
	bool read_reg(uint8_t reg, uint8_t val);


	uint8_t channel;
	uint8_t reference_v;
};


#endif /* MAX11615_CORE_INC_MAX11615_SENS_H_ */
