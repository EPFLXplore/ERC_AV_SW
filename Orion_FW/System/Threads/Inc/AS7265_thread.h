/*
 * AS7265_thread.h
 *
 *      Author: Yassine Bakkali, Vincent Nguyen
 */

#ifndef SENSORS_AS7265_THREAD_H
#define SENSORS_AS7265_THREAD_H


#include <AS7265x.hpp>
#include <DataStructures.h>
#include <Prober.h>
#include <Thread.h>
#include "Telemetry.h"


class AS7265Thread : public Thread {
public:
	AS7265Thread(ProberThread* parent) : Thread("AS7265"), parent(parent), portNum(parent->getI2CNum()), spectro(parent->getI2C(),  AS7265X_ADDR){}
	void init();
	void loop();
	uint8_t getPortNum();

	static void handle_take_measurement(uint8_t sender_id, SpectroPacket* packet);
	void take_measurements();
private:
	ProberThread* parent;
	uint8_t portNum;
	AS7265x spectro;
	SpectroPacket spectro_request_packet;
	SpectroResponsePacket spectro_response_packet;

};

extern AS7265Thread* AS7265Instance;

#endif /* SENSORS_AS7265_THREAD_H */
