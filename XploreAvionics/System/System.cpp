/*
 * System.cpp
 *
 *  Created on: 23 Oct 2020
 *      Author: arion
 */

#include "System.h"

#include "Misc/WatchdogThread.h"
#include "Sensors/Barometer.h"
#include "Sensors/IMU.h"

void initCortexM4() {
	new WatchdogThread();
	new IMUThread();
	new BarometerThread();
}

void initCortexM7() {
	new WatchdogThread();
}