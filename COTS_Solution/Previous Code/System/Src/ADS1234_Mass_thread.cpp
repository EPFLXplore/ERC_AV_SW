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
		delete mass_sensor;
		mass_sensor = nullptr;
		MX_SPI1_Init();
		terminate();
		parent->resetProber();
		return;
	}

	LOG_SUCCESS("Thread successfully created");

	// Sensor related configuration after successfully connected
	mass_sensor->begin();
	mass_sensor->set_offset(AIN1, 263616.4375);
	mass_sensor->set_scale(AIN1, 451.8433);
	mass_sensor->set_offset(AIN2, 263616.4375);
	mass_sensor->set_scale(AIN2, 451.8433);
	mass_sensor->set_offset(AIN3, 263616.4375);
	mass_sensor->set_scale(AIN3, 451.8433);
	mass_sensor->set_offset(AIN4, 263616.4375);
	mass_sensor->set_scale(AIN4, 451.8433);

	request_config();
}

ADS1234Thread::~ADS1234Thread() {
    // Release the dynamically allocated memory for mass_sensor
    delete mass_sensor;
    mass_sensor = nullptr;
}

static MassData mass_data;

static MassPacket mass_packet;

static MassConfigRequestPacket mass_config_packet;

void ADS1234Thread::request_config() {
	LOG_INFO("Requesting configuration...");
	config_time = xTaskGetTickCount();
	mass_config_packet.req_offset = true;
	mass_config_packet.req_scale = true;
	mass_config_packet.req_alpha = true;
	mass_config_packet.req_channels_status = true;
	MAKE_IDENTIFIABLE(mass_config_packet);
	MAKE_RELIABLE_MCU(mass_config_packet);
	Telemetry::set_id(JETSON_NODE_ID);
	FDCAN1_network->send(&mass_config_packet);
	FDCAN2_network->send(&mass_config_packet);
	portYIELD();
}

void ADS1234Thread::start_calib_offset(uint32_t num_samples, uint8_t channel) {
	if (channel <= 4) {
		if (!calibrating_offset) {
			if (!calibrating_scale) {
				calib_channel = channel;
				cnt_mass_offset = 0;
				for (uint8_t i = 0; i < 4; ++i)
					mass_avg_offset[i] = 0;
				calib_samples_offset = num_samples;
				calibrating_offset = true;
				LOG_INFO("Starting offset calibration...");
			} else {
				LOG_ERROR("Cannot calibrate both offset and scale at the same time");
			}
		} else {
			LOG_ERROR("Another offset calibration is already active. Aborting calibration...");
		}
	} else {
		LOG_ERROR("Invalid channel number for offset calibration: %d", channel);
	}
}

void ADS1234Thread::start_calib_scale(uint32_t num_samples, uint8_t channel, float calib_weight) {
	if (channel <= 4) {
		if (!calibrating_scale) {
			if (!calibrating_offset) {
				calib_channel = channel;
				cnt_mass_scale = 0;
				this->calib_weight = calib_weight;
				for (uint8_t i = 0; i < 4; ++i)
					mass_avg_scale[i] = 0;
				calib_samples_scale = num_samples;
				calibrating_scale = true;
				LOG_INFO("Starting scale calibration...");
			} else {
				LOG_ERROR("Cannot calibrate both offset and scale at the same time");
			}
		} else {
			LOG_ERROR("Another scale calibration is already active. Aborting calibration...");
		}
	} else {
		LOG_ERROR("Invalid channel number for scale calibration: %d", channel);
	}
}

static MassConfigResponsePacket mass_calib_offset_response_packet = {};

void ADS1234Thread::send_calib_offset() {
	// Compute average value

	for (uint8_t i = 0; i < 4; ++i) {
		if (this->enabled_channels[i] && (cnt_mass_offset != 0))
			mass_avg_offset[i] = mass_sum_offset[i]/cnt_mass_offset;
	}

	calibrating_offset = false;
	cnt_mass_offset = 0;
	for (uint8_t i = 0; i < 4; ++i)
		mass_sum_offset[i] = 0;

	mass_calib_offset_response_packet.set_offset = true;

	if (this->enabled_channels[0] && ((calib_channel == 1) || (calib_channel == 0)))
		mass_sensor->set_offset(AIN1, mass_avg_offset[0]);

	if (this->enabled_channels[1] || ((calib_channel == 2) || (calib_channel == 0)))
			mass_sensor->set_offset(AIN2, mass_avg_offset[1]);

	if (this->enabled_channels[2] || ((calib_channel == 3) || (calib_channel == 0)))
			mass_sensor->set_offset(AIN3, mass_avg_offset[2]);

	if (this->enabled_channels[3] || ((calib_channel == 4) || (calib_channel == 0)))
			mass_sensor->set_offset(AIN4, mass_avg_offset[3]);


	mass_calib_offset_response_packet.offset[0] = mass_sensor->get_offset(AIN1);
	mass_calib_offset_response_packet.offset[1] = mass_sensor->get_offset(AIN2);
	mass_calib_offset_response_packet.offset[2] = mass_sensor->get_offset(AIN3);
	mass_calib_offset_response_packet.offset[3] = mass_sensor->get_offset(AIN4);

	LOG_SUCCESS("Computed mass sensor offset: [%.3f %.3f %.3f %.3f]",
			mass_sensor->get_offset(AIN1), mass_sensor->get_offset(AIN2),
			mass_sensor->get_offset(AIN3), mass_sensor->get_offset(AIN4));


	for (uint8_t i = 0; i < 4; ++i) {
		mass_calib_offset_response_packet.enabled_channels[i] = enabled_channels[i];
	}

	MAKE_IDENTIFIABLE(mass_calib_offset_response_packet);
	MAKE_RELIABLE_MCU(mass_calib_offset_response_packet);
	Telemetry::set_id(JETSON_NODE_ID);
	if (sender_id == 1)
		FDCAN1_network->send(&mass_calib_offset_response_packet);
	else if (sender_id == 2)
		FDCAN2_network->send(&mass_calib_offset_response_packet);
	portYIELD();

}

static MassConfigResponsePacket mass_calib_scale_response_packet = {};

void ADS1234Thread::send_calib_scale() {
	// Compute average value

	for (uint8_t i = 0; i < 4; ++i) {
		if (this->enabled_channels[i] && (cnt_mass_scale != 0) && (calib_weight != 0)) {
			mass_avg_scale[i] = mass_sum_scale[i]/(cnt_mass_scale*calib_weight);
		}
	}

	calibrating_scale = false;
	cnt_mass_scale = 0;
	for (uint8_t i = 0; i < 4; ++i)
		mass_sum_scale[i] = 0;

	mass_calib_scale_response_packet.set_scale = true;

	if (this->enabled_channels[0] && ((calib_channel == 1) || (calib_channel == 0)))
		mass_sensor->set_scale(AIN1, mass_avg_scale[0]);

	if (this->enabled_channels[1] || ((calib_channel == 2) || (calib_channel)))
			mass_sensor->set_scale(AIN2, mass_avg_scale[1]);

	if (this->enabled_channels[2] || ((calib_channel == 3) || (calib_channel == 0)))
			mass_sensor->set_scale(AIN3, mass_avg_scale[2]);

	if (this->enabled_channels[3] || ((calib_channel == 4) || (calib_channel == 0)))
			mass_sensor->set_scale(AIN4, mass_avg_scale[3]);


	mass_calib_scale_response_packet.scale[0] = mass_sensor->get_scale(AIN1);
	mass_calib_scale_response_packet.scale[1] = mass_sensor->get_scale(AIN2);
	mass_calib_scale_response_packet.scale[2] = mass_sensor->get_scale(AIN3);
	mass_calib_scale_response_packet.scale[3] = mass_sensor->get_scale(AIN4);

	LOG_SUCCESS("Computed mass sensor scale: [%.3f %.3f %.3f %.3f]",
			mass_sensor->get_scale(AIN1), mass_sensor->get_scale(AIN2),
			mass_sensor->get_scale(AIN3), mass_sensor->get_scale(AIN4));

	for (uint8_t i = 0; i < 4; ++i) {
		mass_calib_offset_response_packet.enabled_channels[i] = enabled_channels[i];
	}

	MAKE_IDENTIFIABLE(mass_calib_scale_response_packet);
	MAKE_RELIABLE_MCU(mass_calib_scale_response_packet);
	Telemetry::set_id(JETSON_NODE_ID);
	if (sender_id == 1)
		FDCAN1_network->send(&mass_calib_scale_response_packet);
	else if (sender_id == 2)
		FDCAN2_network->send(&mass_calib_scale_response_packet);
	portYIELD();

}

void ADS1234Thread::set_sender_id(uint8_t sender_id) {
	this->sender_id = sender_id;
}

void ADS1234Thread::loop() {
	// Request configuration
	if((xTaskGetTickCount()-config_time > config_req_interval) && !configured) {
		request_config();
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

		// Calibration
		if(calibrating_offset) {
			cnt_mass_offset += 1;
			mass_sum_offset[0] += mass_sensor->get_last_filtered_raw(AIN1);
			mass_sum_offset[1] += mass_sensor->get_last_filtered_raw(AIN2);
			mass_sum_offset[2] += mass_sensor->get_last_filtered_raw(AIN3);
			mass_sum_offset[3] += mass_sensor->get_last_filtered_raw(AIN4);
		}

		if(calibrating_scale) {
			cnt_mass_scale += 1;
			mass_sum_scale[0] += mass_sensor->get_last_filtered_tared(AIN1);
			mass_sum_scale[1] += mass_sensor->get_last_filtered_tared(AIN2);
			mass_sum_scale[2] += mass_sensor->get_last_filtered_tared(AIN3);
			mass_sum_scale[3] += mass_sensor->get_last_filtered_tared(AIN4);
		}

		if(calibrating_offset && (cnt_mass_offset > calib_samples_offset)) {
			send_calib_offset();
		}

		if(calibrating_scale && (cnt_mass_scale > calib_samples_scale)) {
			send_calib_scale();
		}

		mass_data.toArray((uint8_t*) &mass_packet);
		MAKE_IDENTIFIABLE(mass_packet);
		MAKE_RELIABLE_MCU(mass_packet);
		Telemetry::set_id(JETSON_NODE_ID);
		FDCAN1_network->send(&mass_packet);
		FDCAN2_network->send(&mass_packet);
		portYIELD();
	} else {
		LOG_ERROR("Thread aborted");
		MassSensorInstance = nullptr;
		delete mass_sensor;
		mass_sensor = nullptr;
		if (hspi == &hspi1)
			MX_SPI1_Init();
		else if (hspi == &hspi2)
			MX_SPI2_Init();
		else if (hspi == &hspi3)
			MX_SPI3_Init();
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
	if(!(IS_RELIABLE_MCU(*packet))) {
		console.printf_error("Unreliable mass config packet");
		return;
	}
	mass_config_response_packet.set_offset = packet->set_offset;
	mass_config_response_packet.set_scale = packet->set_scale;
	mass_config_response_packet.set_alpha = packet->set_alpha;
	mass_config_response_packet.set_channels_status = packet->set_channels_status;

	// Set fields to zero
	for (uint8_t i = 0; i < 4; ++i) {
		mass_config_response_packet.enabled_channels[i] = 0;
	}

	for (uint8_t i = 0; i < 4; ++i) {
		mass_config_response_packet.offset[i] = 0;
		mass_config_response_packet.scale[i] = 0;
	}
	mass_config_response_packet.alpha = 0;


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
	MAKE_RELIABLE_MCU(mass_config_response_packet);
	Telemetry::set_id(JETSON_NODE_ID);
	if (sender_id == 1)
		FDCAN1_network->send(&mass_config_response_packet);
	else if (sender_id == 2)
		FDCAN2_network->send(&mass_config_response_packet);
}

void ADS1234Thread::handle_mass_calib(uint8_t sender_id, MassCalibPacket* packet) {
	if(!(IS_RELIABLE_MCU(*packet))) {
		console.printf_error("Unreliable mass calibration packet");
		return;
	}
	mass_calib_offset_response_packet.set_offset = false;
	mass_calib_offset_response_packet.set_scale = false;
	mass_calib_offset_response_packet.set_alpha = false;
	mass_calib_offset_response_packet.set_channels_status = false;

	mass_calib_scale_response_packet.set_offset = false;
	mass_calib_scale_response_packet.set_scale = false;
	mass_calib_scale_response_packet.set_alpha = false;
	mass_calib_scale_response_packet.set_channels_status = false;

	// Set fields to zero (1 for scale to avoid division by zero)
	for (uint8_t i = 0; i < 4; ++i) {
		mass_calib_offset_response_packet.offset[i] = 0;
		mass_calib_offset_response_packet.scale[i] = 1;
		mass_calib_offset_response_packet.enabled_channels[i] = false;

		mass_calib_scale_response_packet.offset[i] = 0;
		mass_calib_scale_response_packet.scale[i] = 1;
		mass_calib_scale_response_packet.enabled_channels[i] = false;
	}

	mass_calib_offset_response_packet.alpha = 0;
	mass_calib_scale_response_packet.alpha = 0;


	if (MassSensorInstance != nullptr) {
		MassSensorInstance->set_sender_id(sender_id);
		if (MassSensorInstance->get_sensor() != nullptr) {
			MassSensorInstance->LOG_INFO("Received ADS1234 calibration command");
			if (packet->calib_offset) {
				if (packet->channel <= 4) {
					if (packet->channel != 0) {
						if (MassSensorInstance->get_channels_status()[packet->channel-1]) {
							MassSensorInstance->start_calib_offset(50, packet->channel);
							mass_calib_offset_response_packet.success = true;
						} else {
							MassSensorInstance->LOG_ERROR("Channel number for offset calibration is not enabled: %d", packet->channel);
							mass_calib_offset_response_packet.success = false;
						}
					} else {
						// if channel = 0, return success = true if at least one of the channels is enabled
						uint8_t num_enabled_channels = 0;
						for (uint8_t i = 0; i < 4; ++i) {
							if (MassSensorInstance->get_channels_status()[i])
								num_enabled_channels++;
						}
						if (num_enabled_channels == 0) {
							mass_calib_offset_response_packet.success = false;
							MassSensorInstance->LOG_ERROR("Unable to calibrate offset since all channels are disabled");
						} else {
							MassSensorInstance->start_calib_offset(50, packet->channel);
							mass_calib_offset_response_packet.success = true;
						}
					}
				} else {
					MassSensorInstance->LOG_ERROR("Invalid channel number for offset calibration: %d", packet->channel);
					mass_calib_offset_response_packet.success = false;
				}
			}
			if (packet->calib_scale) {
				if (packet->expected_weight > 0) {
					if (packet->channel <= 4) {
						if (packet->channel != 0) {
							if (MassSensorInstance->get_channels_status()[packet->channel-1]) {
								MassSensorInstance->start_calib_scale(50, packet->channel, packet->expected_weight);
								mass_calib_scale_response_packet.success = true;
							} else {
								MassSensorInstance->LOG_ERROR("Channel number for scale calibration is not enabled: %d", packet->channel);
								mass_calib_scale_response_packet.success = false;
							}
						} else {
							// if channel = 0, return success = true if at least one of the channels is enabled
							uint8_t num_enabled_channels = 0;
							for (uint8_t i = 0; i < 4; ++i) {
								if (MassSensorInstance->get_channels_status()[i])
									num_enabled_channels++;
							}
							if (num_enabled_channels == 0) {
								MassSensorInstance->LOG_ERROR("Unable to calibrate scale since all channels are disabled");
								mass_calib_scale_response_packet.success = false;
							} else {
								MassSensorInstance->start_calib_scale(50, packet->channel, packet->expected_weight);
								mass_calib_scale_response_packet.success = true;
							}
						}
					} else {
						MassSensorInstance->LOG_ERROR("Invalid channel number for scale calibration: %d", packet->channel);
						mass_calib_scale_response_packet.success = false;
					}
				} else {
					MassSensorInstance->LOG_ERROR("Invalid calibration weight: %+.3f", packet->expected_weight);
					mass_calib_scale_response_packet.success = false;
				}
			}

		} else {
			MassSensorInstance->LOG_ERROR("Mass sensor member non-existent");
			mass_calib_offset_response_packet.success = false;
			mass_calib_scale_response_packet.success = false;
		}


	} else {
		console.printf_error("ADS1234Thread instance does not exist yet\r\n");
		mass_calib_offset_response_packet.success = false;
		mass_calib_scale_response_packet.success = false;
	}


	if ((packet->calib_offset) && (mass_calib_offset_response_packet.success == false)) {
		MAKE_IDENTIFIABLE(mass_calib_offset_response_packet);
		MAKE_RELIABLE_MCU(mass_calib_offset_response_packet);
		Telemetry::set_id(JETSON_NODE_ID);
		if (sender_id == 1)
			FDCAN1_network->send(&mass_calib_offset_response_packet);
		else if (sender_id == 2)
			FDCAN2_network->send(&mass_calib_offset_response_packet);
		portYIELD();
	}

	if ((packet->calib_scale) && (mass_calib_scale_response_packet.success == false)) {
		MAKE_IDENTIFIABLE(mass_calib_scale_response_packet);
		MAKE_RELIABLE_MCU(mass_calib_scale_response_packet);
		Telemetry::set_id(JETSON_NODE_ID);
		if (sender_id == 1)
			FDCAN1_network->send(&mass_calib_scale_response_packet);
		else if (sender_id == 2)
			FDCAN2_network->send(&mass_calib_scale_response_packet);
		portYIELD();
	}
}
