/*
 * NPK_thread.h
 *
 *  Created on: Jul 9, 2023
 *      Author: YassineBakkali
 */

#ifndef SENSORS_NPK_THREAD_H_
#define SENSORS_NPK_THREAD_H_


#include "Thread.h"
#include "Prober.h"
#include "DataStructures.h"
#include "usart.h"

class NPKThread : public Thread {
public:
	NPKThread() :
		Thread("NPK"){};
	void init();
	void loop();
private:
	float dummy_member;
};






#endif /* SENSORS_NPK_THREAD_H_ */
