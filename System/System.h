/*
 * System.h
 *
 *  Created on: Aug 1, 2022
 *      Author: ???
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_


#include "Telemetry.h"
#include "i2c.h"

void initCortex();
Vector bnoVectorToVector(bno055_vector_t v);





#endif /* SYSTEM_H_ */
