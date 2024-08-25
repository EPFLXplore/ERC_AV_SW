/*
 * ESP32_Neopixel_thread.h
 *
 *  Created on: Aug 25, 2024
 *      Author: eliot
 */

#ifndef THREADS_INC_ESP32_NEOPIXEL_THREAD_H_
#define THREADS_INC_ESP32_NEOPIXEL_THREAD_H_

#include <DataStructures.h>
#include <Prober.h>
#include <Thread.h>
#include "Telemetry.h"

#define ESP32_ADDR (0x50)

//#define USE_GPIO // define if we want to send commands to RP2040 with GPIOs instead of I2C


class LEDThread : public Thread {
public:
	LEDThread(ProberThread* parent) : Thread("LED"), parent(parent), portNum(parent->getI2CNum()){}
	void init();
	void loop();
	uint8_t getPortNum();

	static void handle_led_request(uint8_t sender_id, LEDPacket* packet);

private:
	ProberThread* parent;
	uint8_t portNum;
};

extern LEDThread* LEDInstance;

#endif /* THREADS_INC_ESP32_NEOPIXEL_THREAD_H_ */
