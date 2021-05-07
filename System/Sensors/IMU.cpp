/*
 * IMU.cpp
 *
 *  Created on: 7 Nov 2020
 *      Author: AV Team 2020
 */

#include "IMU.h"

#include "Telemetry.h"


static char cbuf[256];

void IMUThread::init() {
	bno055_assignI2C(parent->getI2C());
	bno055_setup();
	bno055_setOperationModeNDOF();

	if(bno055_getSystemError() != BNO055_SYSTEM_ERROR_NO_ERROR) {
		println("[%s] BNO055 initialization failed", portNum);
		terminate();
		parent->resetProber();
		return;
	}

	println("[%s] BNO055 initialized", portNum);
}

static IMUData data;
static Avionics_AccelMagPacket packet;
void IMUThread::loop() {
	data.accel = bnoVectorToVector(bno055_getVectorAccelerometer());
	data.gyro = bnoVectorToVector(bno055_getVectorEuler());
	data.mag = bnoVectorToVector(bno055_getVectorGravity());

	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
		println("[%s] %s", portNum, data.toString(cbuf));
		data.toArray((uint8_t*) &packet);
		network.send(&packet);
		portYIELD();
	} else {
		println("[%s] BNO055 disconnected", portNum);
		terminate();
		parent->resetProber();
	}
}


Vector IMUThread::bnoVectorToVector(bno055_vector_t v) {
	Vector vector;

	vector.x = v.x;
	vector.y = v.y;
	vector.z = v.z;

	return vector;
}
