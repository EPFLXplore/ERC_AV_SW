/*
 * LWIPThread.h
 *
 *  Created on: Jan 23, 2021
 *      Author: arion
 */

#ifndef TELEMETRY_LWIPTHREAD_H_
#define TELEMETRY_LWIPTHREAD_H_

#include "Thread.h"

#include <stdint.h>

class LWIPThread : public Thread {
public:
	LWIPThread(const char* ip, const uint16_t port);
	void init();
	void loop();

private:
	const char* ip;
	const uint16_t port;
};


#endif /* TELEMETRY_LWIPTHREAD_H_ */
