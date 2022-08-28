/*
 * IMU.cpp
 *
 *  Created on: 7 Nov 2020
 *      Author: AV Team 2020
 */

#include "IMU.h"

#include "Telemetry.h"

//static char cbuf[256];

void IMUThread::init() {
	bno055_assignI2C(parent->getI2C());
	bno055_setup();
	bno055_setOperationModeNDOF();

	if(bno055_getSystemError() != BNO055_SYSTEM_ERROR_NO_ERROR) {
//		println("[%d] BNO055 initialization failed", portNum);
		terminate();
		parent->resetProber();
		return;
	}

//	println("[%d] BNO055 initialized", portNum);
}

static IMUData data;
static avionics_IMU_packet packet;
void IMUThread::loop() {
	data.accel = bnoVectorToVector(bno055_getVectorAccelerometer());
	data.gyro = bnoVectorToVector(bno055_getVectorGyroscope());
//	data.mag = bnoVectorToVector(bno055_getVectorMagnetometer());
	data.orientation = bnoQuaternionToQuaternion(bno055_getVectorQuaternion());

	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
//		println("[i2c%d] %s", portNum, data.toString(cbuf));
		data.toArray((uint8_t*) &packet);
		network.send(&packet);
		portYIELD();
	} else {
//		println("[i2c%d] BNO055 disconnected", portNum);
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

