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

}

ADS1234Thread::~ADS1234Thread() {
    // Release the dynamically allocated memory for mass_sensor
    delete mass_sensor;
    mass_sensor = nullptr;
}

static MassData mass_data;

static MassPacket mass_packet;


void ADS1234Thread::set_sender_id(uint8_t sender_id) {
	this->sender_id = sender_id;
}

void ADS1234Thread::loop() {
	
	// Calibrate every 90 seconds
	ERROR_t err_ch1 = NoERROR;
	ERROR_t err_ch2 = NoERROR;
	ERROR_t err_ch3 = NoERROR;
	ERROR_t err_ch4 = NoERROR;

	if (enabled_channels[0])
    	err_ch1 = mass_sensor->get_units(AIN1, mass_data.mass[0], alpha, calibrating);

	if (enabled_channels[1])
    	err_ch2 = mass_sensor->get_units(AIN2, mass_data.mass[1], alpha, calibrating);

	if (enabled_channels[2])
    	err_ch3 = mass_sensor->get_units(AIN3, mass_data.mass[2], alpha, calibrating);

	if (enabled_channels[3])
    	err_ch4 = mass_sensor->get_units(AIN4, mass_data.mass[3], alpha, calibrating);

    if (calibrating) {
    	calibrating = false;
    }

	if((err_ch1 == NoERROR) && (err_ch2 == NoERROR) && (err_ch3 == NoERROR) && (err_ch4 == NoERROR)) {
		if(monitor.enter(MASS_MONITOR)) {
			println("%s", mass_data.toString(cbuf));
		}

		//Reading 
		mass_data.mass[0] = mass_sensor->get_last_filtered_raw(AIN1);
		mass_data.mass[1] = mass_sensor->get_last_filtered_raw(AIN2);
		mass_data.mass[2] = mass_sensor->get_last_filtered_raw(AIN3);
		mass_data.mass[3] = mass_sensor->get_last_filtered_raw(AIN4);

		mass_packet.mass[0] = mass_data.mass[0];
		mass_packet.mass[1] = mass_data.mass[1];
		mass_packet.mass[2] = mass_data.mass[2];
		mass_packet.mass[3] = mass_data.mass[3];

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

static MassConfigResponsePacket mass_config_response_packet = {};

void ADS1234Thread::handle_set_config(uint8_t sender_id, MassConfigPacket* packet) {
	if(!(IS_RELIABLE_MCU(*packet))) {
		console.printf_error("Unreliable mass config packet");
		return;
	}
}

void ADS1234Thread::handle_mass_calib(uint8_t sender_id, MassCalibPacket* packet) {
	if(!(IS_RELIABLE_MCU(*packet))) {
		console.printf_error("Unreliable mass calibration packet");
		return;
	}
}
