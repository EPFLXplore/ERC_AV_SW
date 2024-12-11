/*
 * FDCAN_State_Indicator_thread.h
 *
 *  Created on: Aug 17, 2023
 *      Author: Vincent Nguyen
 */

#ifndef THREADS_INC_FDCAN_STATE_INDICATOR_THREAD_H_
#define THREADS_INC_FDCAN_STATE_INDICATOR_THREAD_H_

#include "Thread.h"
#include "fdcan.h"
#include "DataStructures.h"

class FDCANIndicatorThread : public Thread {
public:
	FDCANIndicatorThread(FDCAN_HandleTypeDef* fdcan) : Thread("FDCANIndicator"), fdcan(fdcan) {}
	void init();
	void loop();
	void send_dummy();
private:
	FDCAN_HandleTypeDef* fdcan;
	bool enabled = false;
};

extern FDCANIndicatorThread* FDCAN_Indicator_Instance;




#endif /* THREADS_INC_FDCAN_STATE_INDICATOR_THREAD_H_ */
