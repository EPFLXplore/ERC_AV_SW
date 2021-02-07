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
	bno055_assignI2C(this->hi2c);
	bno055_setup();
	bno055_setOperationModeNDOF();

	BaroData data;
	println("%s", data.toString(cbuf));


	while(bno055_getSystemError() != BNO055_SYSTEM_ERROR_NO_ERROR) {
		osDelay(500);
		println("BNO055 initialization failed");

		bno055_assignI2C(this->hi2c);
		bno055_setup();
		bno055_setOperationModeNDOF();
	}

	println("BNO055 initialized");
}

void IMUThread::loop() {
	static IMUData data;

	data.accel = bnoVectorToVector(bno055_getVectorAccelerometer());
	data.gyro = bnoVectorToVector(bno055_getVectorEuler());
	data.mag = bnoVectorToVector(bno055_getVectorGravity());

	println("%s", data.toString(cbuf));

	Avionics_AccelMagPacket packet;
	data.toArray((float*) &packet);

	network.send(&packet);

	osDelay(100);
}


Vector IMUThread::bnoVectorToVector(bno055_vector_t v) {
	Vector vector;

	vector.x = v.x;
	vector.y = v.y;
	vector.z = v.z;

	return vector;
}
