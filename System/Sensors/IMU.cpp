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

	while(bno055_getSystemError() != BNO055_SYSTEM_ERROR_NO_ERROR) {
		println("BNO055 initialization failed");
		parent->resetProber();
	}

	println("BNO055 initialized");
}

static IMUData data;
static Avionics_AccelMagPacket packet;
void IMUThread::loop() {
	data.accel = bnoVectorToVector(bno055_getVectorAccelerometer());
	data.gyro = bnoVectorToVector(bno055_getVectorEuler());
	data.mag = bnoVectorToVector(bno055_getVectorGravity());

	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
		println("%s", data.toString(cbuf));
		data.toArray((uint8_t*) &packet);
		network.send(&packet);
		portYIELD();
	} else {
		println("BNO055 disconnected");
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
