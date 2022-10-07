/*
 * IMU.cpp
 *
 *  Created on: 7 Nov 2020
 *      Author: AV Team 2020
 */

#include "IMU.h"

#include "Telemetry.h"


void IMUThread::init() {
	bno055_assignI2C(parent->getI2C());
	bno055_setup();
	bno055_setOperationModeNDOF();

	if(bno055_getSystemError() != BNO055_SYSTEM_ERROR_NO_ERROR) {
		terminate();
		parent->resetProber();
		return;
	}

}

static IMUData data;
static avionics_IMU_packet packet;
void IMUThread::loop() {
	data.accel = bnoVectorToVector(bno055_getVectorAccelerometer());
	data.gyro = bnoVectorToVector(bno055_getVectorGyroscope());
	data.orientation = bnoQuaternionToQuaternion(bno055_getVectorQuaternion());

	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
		data.toArray((uint8_t*) &packet);
		network.send(&packet);
		portYIELD();
	} else {
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

Quaternion IMUThread::bnoQuaternionToQuaternion(bno055_vector_t v) {
	Quaternion vector;

	vector.w = v.w;
	vector.x = v.x;
	vector.y = v.y;
	vector.z = v.z;

	return vector;
}

