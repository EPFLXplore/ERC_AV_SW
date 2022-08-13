/*
 * Stemma_thread.h
 *
 *  Created on: 10 august 2022
 *      Author: YassineBakkali
 */

#ifndef SENSORS_STEMMA_THREAD_H_
#define SENSORS_STEMMA_THREAD_H_


#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"
#include "stemma.h"

class StemmaThread : public Thread {
public:
	StemmaThread(ProberThread* parent) : Thread("Stemma"), parent(parent), portNum(parent->getI2CNum()) {}
	void init();
	void loop();

private:
	moist_meter stemma;
	ProberThread* parent;
	uint8_t portNum;
};

#endif /* SENSORS_STEMMA_THREAD_H_ */
