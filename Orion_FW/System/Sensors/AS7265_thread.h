/*
 * Voltmeter_thread.h
 *
 *  Created on: 10 Aug 2022
 *      Author: Yassine
 */

#ifndef SENSORS_AS7265_THREAD_H
#define SENSORS_AS7265_THREAD_H


#include <AS7265x.hpp>
#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"


class AS7265Thread : public Thread {
public:
	AS7265Thread(ProberThread* parent) : Thread("AS7265"), parent(parent), portNum(parent->getI2CNum()), spectro(parent->getI2C(),  AS7265X_ADDR){}
	void init();
	void loop();
private:
	AS7265x spectro;
	ProberThread* parent;
	uint8_t portNum;

};

extern AS7265Thread* AS7265Instance;

#endif /* SENSORS_AS7265_THREAD_H */
