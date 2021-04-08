/*
 * Barometer.h
 *
 *  Created on: 7 Nov 2020
 *      Author: AV Team 2020
 */

#ifndef SENSORS_BAROMETER_H_
#define SENSORS_BAROMETER_H_

#include "Prober.h"
#include "Thread.h"

#include "DataStructures.h"
#include "Libraries/BMP280/bmp280.h"


class BarometerThread : public Thread {
public:
	BarometerThread(ProberThread* parent) : Thread("Barometer", 512), bmp280(), parent(parent) {}
	void init();
	void loop();

private:
	BMP280_HandleTypedef bmp280;
	ProberThread* parent;
};


#endif /* SENSORS_BAROMETER_H_ */
