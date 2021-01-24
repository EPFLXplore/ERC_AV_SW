/*
 * profiler.h
 *
 *  Created on: 24 Aug 2020
 *      Author: AV Team 2020
 */

#ifndef APPLICATION_HOSTBOARD_INC_DEBUG_PROFILER_H_
#define APPLICATION_HOSTBOARD_INC_DEBUG_PROFILER_H_


#include <stdint.h>


#define NUM_PROFILERS 16
#define MAX_PROFILER_DEPTH 8
#define TIME_BETWEEN_UPDATES 1000



struct ThreadProfiler {
	uint8_t depth;
	uint32_t last_update;
	int8_t depth_to_display;
	uint32_t timestamps[MAX_PROFILER_DEPTH];
	uint8_t measurement_id[MAX_PROFILER_DEPTH];
};

class Profiler {
public:
	void start(uint8_t measurement_id);
	void end();
	void enable();
	void disable();
private:
	ThreadProfiler profilers[NUM_PROFILERS];

	volatile bool profiling_requested;
	bool profiling;
};


#endif /* APPLICATION_HOSTBOARD_INC_DEBUG_PROFILER_H_ */
