/*
 * LWIPThread.h
 *
 *  Created on: Jan 23, 2021
 *      Author: AV Team 2020
 */

#ifndef TELEMETRY_LWIPTHREAD_H_
#define TELEMETRY_LWIPTHREAD_H_

#include "Thread.h"

#include "Libraries/RoCo/RoCo.h"

#include <stdint.h>


class LWIPThread : public Thread {
public:
	LWIPThread(const char* ip, const uint16_t port);
	~LWIPThread();
	void init();
	void loop();
};


#endif /* TELEMETRY_LWIPTHREAD_H_ */
