/*
 * System.cpp
 *
 *  Created on: 23 Oct 2020
 *      Author: AV Team 2020
 */

#include "System.h"

#include "Lang/Operators.h"

#include "Misc/WatchdogThread.h"
#include "Sensors/Prober.h"
#include "Telemetry/LWIPThread.h"

#include "Debug/Debug.h"


#include "iwdg.h"
#include "usart.h"


#ifdef CORE_CM4
#include "i2c.h"
void initCortexM4() {
	static WatchdogThread watchdog(&hiwdg2);
	static Shell shell(&huart3, &terminal);

	static ProberThread prober1(&hi2c1);
	static ProberThread prober2(&hi2c2);
	static ProberThread prober3(&hi2c4);
}
#endif

#ifdef CORE_CM7
#include "i2c.h"
#include "Telemetry.h"
void initCortexM7() {
	MX_I2C1_Init();

	static WatchdogThread watchdog(&hiwdg1);
	static LWIPThread lwip("192.168.1.2", 42666);

	setupTelemtry();
}
#endif

extern "C" {

void vApplicationStackOverflowHook() {
	while(1);
}

}
