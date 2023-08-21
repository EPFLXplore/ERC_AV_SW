/*
 * ADS1234_thread.hpp
 *
 *  Created on: Apr 2, 2023
 *      Author: Yassine Bakkali, Vincent Nguyen
 */

#ifndef SENSORS_ADS1234_MASS_THREAD_HPP_
#define SENSORS_ADS1234_MASS_THREAD_HPP_


#include <ADS1234.hpp>
#include <DataStructures.h>
#include <Prober.h>
#include <Thread.h>

#define ADS1234_HAT_ADDR (0x4E)

// Enables the channels w.r.t. number of expected load cells
// More channels will also decrease the maximum output data rate
//#define CH1_ENABLE
#define CH2_ENABLE
//#define CH3_ENABLE
//#define CH4_ENABLE
#define USE_LOW_PASS_FILTER

// For plotting with SWV Data Trace Timeline Graph
//#define PLOT_CH1
//#define PLOT_CH2
//#define PLOT_CH3
//#define PLOT_CH4


class ADS1234Thread : public Thread {
public:
	ADS1234Thread(ProberThread* parent, SPI_HandleTypeDef* hspi_);
	~ADS1234Thread();
	void init();
	void loop();
	uint8_t getPortNum();
	ADS1234* get_sensor();
	static void handle_set_config(uint8_t sender_id, MassConfigPacket* packet);

	bool configured = false;

private:
	ProberThread* parent;
	uint8_t portNum;
	SPI_HandleTypeDef* hspi;
	ADS1234* mass_sensor;
	float alpha = 0.8;
	uint16_t  num_averages = 10;
	long unsigned int start = 0;
	bool calibrating = true;

	long unsigned int config_time = 0;
	long unsigned int config_req_interval = 5000;
};

extern ADS1234Thread* MassSensorInstance;




#endif /* SENSORS_ADS1234_MASS_THREAD_HPP_ */
