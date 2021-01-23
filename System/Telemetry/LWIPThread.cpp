/*
 * LWIPThread.cpp
 *
 *  Created on: Jan 23, 2021
 *      Author: arion
 */

#include "LWIPThread.h"

#include "Debug/Debug.h"

#include <lwip.h>


LWIPThread::LWIPThread(const char* ip, const uint16_t port) : Thread("Client thread"), ip(ip), port(port) {

}

void LWIPThread::init() {
	osDelay(50); // Time to get the shell loaded by Cortex M4

	console.printf("Initializing LWIP... ");
	MX_LWIP_Init();
	osDelay(2000);
	console.printf("done\n");
}

void LWIPThread::loop() {

}
