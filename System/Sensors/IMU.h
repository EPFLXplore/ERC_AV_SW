/*
 * IMU.h
 *
 *  Created on: 7 Nov 2020
 *      Author: AV Team 2020
 */

#ifndef SENSORS_IMU_H_
#define SENSORS_IMU_H_


#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"

#include "Libraries/BNO055/bno055_stm32.h"
#include "Libraries/BNO055/bno055.h"

class IMUThread : public Thread {
public:
	IMUThread(ProberThread* parent) : Thread("IMU"), parent(parent), portNum(parent->getI2CNum()) {}
	void init();
	void loop();

private:
	ProberThread* parent;
	Vector bnoVectorToVector(bno055_vector_t v);
	uint8_t portNum;
};

#endif /* SENSORS_IMU_H_ */
