/*
 * Barometer.h
 *
 *  Created on: 7 Nov 2020
 *      Author: arion
 */

#ifndef SENSORS_BAROMETER_H_
#define SENSORS_BAROMETER_H_

#include "Thread.h"

class BarometerThread : Thread {
public:
	BarometerThread() : Thread("Barometer") {}
	void init();
	void loop();
};


#endif /* SENSORS_BAROMETER_H_ */
