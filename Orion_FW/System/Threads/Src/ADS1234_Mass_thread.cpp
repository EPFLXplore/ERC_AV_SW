/*
 * ADS1234_thread.cpp
 *
 *  Created on: Apr 2, 2023
 *      Author: Yassine Bakkali, Vincent Nguyen
 */

#include <ADS1234_Mass_thread.hpp>
#include "Telemetry.h"

#include "Debug.h"

ADS1234Thread* MassSensorInstance = nullptr;
static char cbuf[256]; // for printf

#ifdef PLOT_CH1
float global_mass_ch1 = 0;
#endif

#ifdef PLOT_CH2
float global_mass_ch2 = 0;
#endif

#ifdef PLOT_CH3
float global_mass_ch3 = 0;
#endif

#ifdef PLOT_CH4
float global_mass_ch4 = 0;
#endif

ADS1234Thread::ADS1234Thread(ProberThread* parent, SPI_HandleTypeDef* hspi_) :
		Thread("MassSensor"),
		parent(parent),
		portNum(parent->getI2CNum()),
		hspi(hspi_),
		mass_sensor(nullptr){
    // You can also perform additional initialization steps here if needed.
	if (hspi_ == &hspi1) {
		mass_sensor = new ADS1234(hspi_,
						GPIOA, GPIO_PIN_6, // SPI1_MISO
						GPIOA, GPIO_PIN_5, // SPI1_SCK
						HAT1_P6_GPIO_Port, HAT1_P6_Pin,
						HAT1_P5_GPIO_Port, HAT1_P5_Pin,
						HAT1_P3_GPIO_Port, HAT1_P3_Pin,
						HAT1_P4_GPIO_Port, HAT1_P4_Pin,
						HAT1_P1_GPIO_Port, HAT1_P1_Pin,
						HAT1_P2_GPIO_Port, HAT1_P2_Pin);
	} else if (hspi_ == &hspi2) {
		mass_sensor = new ADS1234(hspi_,
						GPIOC, GPIO_PIN_2, // SPI2_MISO
						GPIOD, GPIO_PIN_3, // SPI2_SCK
						HAT2_P6_GPIO_Port, HAT2_P6_Pin,
						HAT2_P5_GPIO_Port, HAT2_P5_Pin,
						HAT2_P3_GPIO_Port, HAT2_P3_Pin,
						HAT2_P4_GPIO_Port, HAT2_P4_Pin,
						HAT2_P1_GPIO_Port, HAT2_P1_Pin,
						HAT2_P2_GPIO_Port, HAT2_P2_Pin);
	} else if (hspi_ == &hspi3) {
		mass_sensor = new ADS1234(hspi_,
						GPIOB, GPIO_PIN_4, // SPI3_MISO
						GPIOC, GPIO_PIN_10, // SPI3_SCK
						HAT3_P6_GPIO_Port, HAT3_P6_Pin,
						HAT3_P5_GPIO_Port, HAT3_P5_Pin,
						HAT3_P3_GPIO_Port, HAT3_P3_Pin,
						HAT3_P4_GPIO_Port, HAT3_P4_Pin,
						HAT3_P1_GPIO_Port, HAT3_P1_Pin,
						HAT3_P2_GPIO_Port, HAT3_P2_Pin);
	}

}

void ADS1234Thread::init() {
	MassSensorInstance = this;
	// Initialize the sensor
//	ADS1113 dummy_sensor(parent->getI2C(), ADS_ADDR_GND);
	bool success = 1;
	// If the sensor was not found or uncorrectly initialized, reset prober
	if(!success) {
		MassSensorInstance = nullptr;
		MX_SPI1_Init();
		terminate();
		parent->resetProber();
		delete mass_sensor;
		mass_sensor = nullptr;
		return;
	}

	LOG_SUCCESS("Thread successfully created");

	// Sensor related configuration after successfully connected
	mass_sensor->begin();
	mass_sensor->set_offset(AIN1, 265542);
	mass_sensor->set_scale(AIN1, 452.05);
	mass_sensor->set_offset(AIN2, 265542);
	mass_sensor->set_scale(AIN2, 452.05);
	mass_sensor->set_offset(AIN3, 265542);
	mass_sensor->set_scale(AIN3, 452.05);
	mass_sensor->set_offset(AIN4, 265542);
	mass_sensor->set_scale(AIN4, 452.05);
}

ADS1234Thread::~ADS1234Thread() {
    // Release the dynamically allocated memory for mass_sensor
    delete mass_sensor;
    mass_sensor = nullptr;
}

static MassData mass_data;

static MassPacket mass_packet;

static MassConfigRequestPacket mass_config_packet;

void ADS1234Thread::loop() {
	// Request configuration
	if((xTaskGetTickCount()-config_time > config_req_interval) && !configured) {
		LOG_INFO("Requesting configuration...");
		config_time = xTaskGetTickCount();
		mass_config_packet.req_offset = true;
		mass_config_packet.req_scale = true;
		mass_config_packet.req_alpha = true;
		mass_config_packet.req_channels_status = true;
		MAKE_IDENTIFIABLE(mass_config_packet);
		Telemetry::set_id(JETSON_NODE_ID);
		FDCAN1_network->send(&mass_config_packet);
		FDCAN2_network->send(&mass_config_packet);
		portYIELD();
	}


	// Calibrate every 90 seconds
	ERROR_t err_ch1 = NoERROR;
	ERROR_t err_ch2 = NoERROR;
	ERROR_t err_ch3 = NoERROR;
	ERROR_t err_ch4 = NoERROR;
//    if(xTaskGetTickCount()-start > 90000){
//    	calibrating = true;
//    	start = xTaskGetTickCount();
//    	LOG_INFO("Calibrating mass sensor...");
//    }
#ifdef USE_LOW_PASS_FILTER
	if (enabled_channels[0])
    	err_ch1 = mass_sensor->get_units(AIN1, mass_data.mass[0], alpha, calibrating);

	if (enabled_channels[1])
    	err_ch2 = mass_sensor->get_units(AIN2, mass_data.mass[1], alpha, calibrating);

	if (enabled_channels[2])
    	err_ch3 = mass_sensor->get_units(AIN3, mass_data.mass[2], alpha, calibrating);

	if (enabled_channels[3])
    	err_ch4 = mass_sensor->get_units(AIN4, mass_data.mass[3], alpha, calibrating);
#elif
	if (enabled_channels[0])
    	err_ch1 = mass_sensor->get_units(AIN1, mass_data.mass[0], num_averages, calibrating);

	if (enabled_channels[1])
    	err_ch2 = mass_sensor->get_units(AIN2, mass_data.mass[1], num_averages, calibrating);

	if (enabled_channels[2])
    	err_ch3 = mass_sensor->get_units(AIN3, mass_data.mass[2], num_averages, calibrating);

	if (enabled_channels[3])
    	err_ch4 = mass_sensor->get_units(AIN4, mass_data.mass[3], num_averages, calibrating);
#endif

#ifdef PLOT_CH1
    global_mass_ch1 = mass_data.mass[0];
#endif
#ifdef PLOT_CH2
    global_mass_ch2 = mass_data.mass[1];
#endif
#ifdef PLOT_CH3
    global_mass_ch3 = mass_data.mass[2];
#endif
#ifdef PLOT_CH4
    global_mass_ch2 = mass_data.mass[3];
#endif

    if (calibrating) {
    	calibrating = false;
    }

	if((err_ch1 == NoERROR) && (err_ch2 == NoERROR) && (err_ch3 == NoERROR) && (err_ch4 == NoERROR)) {
		if(monitor.enter(MASS_MONITOR)) {
			println("%s", mass_data.toString(cbuf));
		}

		mass_data.toArray((uint8_t*) &mass_packet);
		MAKE_IDENTIFIABLE(mass_packet);
		Telemetry::set_id(JETSON_NODE_ID);
		FDCAN1_network->send(&mass_packet);
		FDCAN2_network->send(&mass_packet);
		portYIELD();
	} else {
		LOG_ERROR("Thread aborted");
		MassSensorInstance = nullptr;
		if (hspi == &hspi1)
			MX_SPI1_Init();
		else if (hspi == &hspi2)
			MX_SPI2_Init();
		else if (hspi == &hspi3)
			MX_SPI3_Init();
		delete mass_sensor;
		mass_sensor = nullptr;
		terminate();
		parent->resetProber();
	}
}

uint8_t ADS1234Thread::getPortNum() {
	return portNum;
}

ADS1234* ADS1234Thread::get_sensor() {
	return mass_sensor;
}

void ADS1234Thread::set_channels_status(bool state[4]) {
	for (uint8_t i = 0; i < 4; ++i) {
		enabled_channels[i] = state[i];
	}
}

const bool* ADS1234Thread::get_channels_status() const {
	return enabled_channels;
}

void ADS1234Thread::set_alpha(float alpha_) {
	alpha = alpha_;
}

float ADS1234Thread::get_alpha() {
	return alpha;
}


static MassConfigResponsePacket mass_config_response_packet = {};

void ADS1234Thread::handle_set_config(uint8_t sender_id, MassConfigPacket* packet) {
	mass_config_response_packet.remote_command = packet->remote_command;
	mass_config_response_packet.set_offset = packet->set_offset;
	mass_config_response_packet.set_scale = packet->set_scale;
	mass_config_response_packet.set_alpha = packet->set_alpha;
	mass_config_response_packet.set_channels_status = packet->set_channels_status;

	if (MassSensorInstance != nullptr) {
		if (packet->remote_command || !(MassSensorInstance->configured)) {
			if (MassSensorInstance->get_sensor() != nullptr) {
				MassSensorInstance->configured = true;
				if (packet->set_offset) {
					MassSensorInstance->get_sensor()->set_offset(AIN1, packet->offset[0]);
					MassSensorInstance->get_sensor()->set_offset(AIN2, packet->offset[1]);
					MassSensorInstance->get_sensor()->set_offset(AIN3, packet->offset[2]);
					MassSensorInstance->get_sensor()->set_offset(AIN4, packet->offset[3]);
					MassSensorInstance->LOG_SUCCESS("Offset configuration set: [%.3f, %.3f, %.3f, %.3f]",
													packet->offset[0], packet->offset[1],
													packet->offset[2], packet->offset[3]);
				}
				if (packet->set_scale) {
					MassSensorInstance->get_sensor()->set_scale(AIN1, packet->scale[0]);
					MassSensorInstance->get_sensor()->set_scale(AIN2, packet->scale[1]);
					MassSensorInstance->get_sensor()->set_scale(AIN3, packet->scale[2]);
					MassSensorInstance->get_sensor()->set_scale(AIN4, packet->scale[3]);
					MassSensorInstance->LOG_SUCCESS("Scale configuration set: [%.3f, %.3f, %.3f, %.3f]",
													packet->scale[0], packet->scale[1],
													packet->scale[2], packet->scale[3]);
				}
				if (packet->set_alpha) {
					MassSensorInstance->set_alpha(packet->alpha);
					MassSensorInstance->LOG_SUCCESS("Low pass filter coefficient (alpha) set: %.3f", packet->alpha);
				}
				if (packet->set_channels_status) {
					MassSensorInstance->set_channels_status(packet->enabled_channels);
					MassSensorInstance->LOG_SUCCESS("Channels status set: [%s, %s, %s, %s]",
													packet->enabled_channels[0] ? "enabled" : "disabled",
													packet->enabled_channels[1] ? "enabled" : "disabled",
													packet->enabled_channels[2] ? "enabled" : "disabled",
													packet->enabled_channels[3] ? "enabled" : "disabled");
				}
				mass_config_response_packet.success = true;
			} else {
				mass_config_response_packet.success = false;
				MassSensorInstance->LOG_ERROR("Mass sensor member non-existent");
			}
		} else {
			mass_config_response_packet.success = false;
			MassSensorInstance->LOG_ERROR("Configuration already requested");
		}
		const bool* enabled_channels = MassSensorInstance->get_channels_status();
		for (uint8_t i = 0; i < 4; ++i) {
			mass_config_response_packet.enabled_channels[i] = enabled_channels[i];
		}

		mass_config_response_packet.offset[0] = MassSensorInstance->get_sensor()->get_offset(AIN1);
		mass_config_response_packet.offset[1] = MassSensorInstance->get_sensor()->get_offset(AIN2);
		mass_config_response_packet.offset[2] = MassSensorInstance->get_sensor()->get_offset(AIN3);
		mass_config_response_packet.offset[3] = MassSensorInstance->get_sensor()->get_offset(AIN4);
		mass_config_response_packet.scale[0] = MassSensorInstance->get_sensor()->get_scale(AIN1);
		mass_config_response_packet.scale[1] = MassSensorInstance->get_sensor()->get_scale(AIN2);
		mass_config_response_packet.scale[2] = MassSensorInstance->get_sensor()->get_scale(AIN3);
		mass_config_response_packet.scale[3] = MassSensorInstance->get_sensor()->get_scale(AIN4);
		mass_config_response_packet.alpha = MassSensorInstance->get_alpha();
	} else {
		mass_config_response_packet.success = false;
		console.printf_error("ADS1234Thread instance does not exist yet\r\n");
	}
	MAKE_IDENTIFIABLE(mass_config_response_packet);
	Telemetry::set_id(JETSON_NODE_ID);
	if (sender_id == 1)
		FDCAN1_network->send(&mass_config_response_packet);
	else if (sender_id == 2)
		FDCAN2_network->send(&mass_config_response_packet);
}
