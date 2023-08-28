/*
 * RP2040_LED_thread.h
 *
 *  Created on: Aug 27, 2023
 *      Author: Vincent
 */

#ifndef THREADS_INC_RP2040_LED_THREAD_H_
#define THREADS_INC_RP2040_LED_THREAD_H_

#include <DataStructures.h>
#include <Prober.h>
#include <Thread.h>
#include "Telemetry.h"

#define RP2040_ADDR (0x50)


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


#endif /* THREADS_INC_RP2040_LED_THREAD_H_ */
