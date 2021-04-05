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
#include "Sensors/ADC24.h"
#include "Sensors/ADC16.h"
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
	static ADC24Thread scale(GPIOB, GPIO_PIN_10, GPIOB, GPIO_PIN_11); //Same pins as i2c2
	static ADC16Thread potentiometer(&hi2c1);
}
#endif

#ifdef CORE_CM7
#include "i2c.h"
#include "Telemetry.h"
void initCortexM7() {
	MX_I2C1_Init();

	static WatchdogThread watchdog(&hiwdg1);
	static LWIPThread lwip("192.168.1.2", 42666);
	//static BarometerThread barometer(&hi2c1);

	setupTelemtry();
}
#endif

extern "C" {

void vApplicationStackOverflowHook() {
	while(1);
}

}
