/*
 * Bno.h
 *
 *  Created on: Nov 6, 2020
 *      Author: jthom
 */

#ifndef BNO_H_
#define BNO_H_

#include "bno055_stm32.h"

class Bno{
public:
	Bno();
	init();
	getOrientation();
	getAcc();

private:
	I2C_HandleTypeDef *_bno055_i2c_port;
};

#endif /* INC_BNO_H_ */
