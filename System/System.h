/*
 * System.h
 *
 *  Created on: 23 Oct 2020
 *      Author: arion
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#ifdef __cplusplus
#include "Thread.hpp"
#include "Lang/Operators.h"

extern "C" {
#endif

void initCortexM4(I2C_HandleTypeDef* hi2c1);
void initCortexM7();

#ifdef __cplusplus
}
#endif


#endif /* SYSTEM_H_ */
