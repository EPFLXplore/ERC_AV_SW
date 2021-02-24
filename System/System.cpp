/*
 * System.cpp
 *
 *  Created on: 23 Oct 2020
 *      Author: AV Team 2020
 */

#include "System.h"

#include "Lang/Operators.h"

#include "Misc/WatchdogThread.h"
#include "Sensors/Barometer.h"
#include "Sensors/IMU.h"
#include "Telemetry/LWIPThread.h"

#include "Debug/Debug.h"


#include "iwdg.h"
#include "usart.h"


#ifdef CORE_CM4
#include "i2c.h"
void initCortexM4() {
	static WatchdogThread watchdog(&hiwdg2);
	static Shell shell(&huart3, &terminal);
	static IMUThread imu(&hi2c1);
	static BarometerThread barometer(&hi2c1);
}
#endif

#ifdef CORE_CM7
void initCortexM7() {
	static WatchdogThread watchdog(&hiwdg1);
	static LWIPThread lwip("169.254.227.123", 42666);
}
#endif
