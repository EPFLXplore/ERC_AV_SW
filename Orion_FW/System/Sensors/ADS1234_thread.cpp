/*
 * ADS1234_thread.cpp
 *
 *  Created on: Apr 2, 2023
 *      Author: Yassine Bakkali
 */

#include "ADS1234_thread.hpp"

#include "Telemetry.h"

ADS1234Thread* massSensorInstance = nullptr;
static char cbuf[256]; // for printf

ADS1234Thread::ADS1234Thread(ProberThread* parent, SPI_HandleTypeDef* hspi_) : Thread("ADS1234"), parent(parent), portNum(parent->getI2CNum()), mass_sensor(nullptr){
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
	massSensorInstance = this;
	// Initialize the sensor
//	ADS1113 dummy_sensor(parent->getI2C(), ADS_ADDR_GND);
	bool success = 1;
	// If the sensor was not found or uncorrectly initialized, reset prober
	if(!success) {
		massSensorInstance = nullptr;
		MX_SPI1_Init();
		terminate();
		parent->resetProber();
		return;
	}

	// Sensor related configuration after successfully connected
	mass_sensor->begin();
	mass_sensor->set_offset(AIN3, 265542);
	mass_sensor->set_scale(AIN3, 452.05);
//	mass_sensor.get_value(AIN3, mass_value, 0.8, true);
//	long dummy;
//	mass_sensor.read(AIN3, dummy, true);
}

ADS1234Thread::~ADS1234Thread() {
    // Release the dynamically allocated memory for mass_sensor
    delete mass_sensor;
}

// Declare your data with the proper data structure defined in DataStructures.h
static MassData mass_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
static MassPacket packet;

void ADS1234Thread::loop() {
	// Get the sensor data. Here we only read a differential value as an example
//	mass_sensor.read(AIN3,mass_value[0],0);
	mass_sensor->get_units(AIN3, mass_value, 10, false);
//	mass_sensor.read_filtered(AIN3, mass_value, 0.5, false);
	mass_data.mass = mass_value;
	// We can print it to SVW console (optional)
	printf("Diff value %s \n", mass_data.toString(cbuf));

	if(HAL_I2C_GetError(parent->getI2C()) == HAL_I2C_ERROR_NONE) {
		// Send data over RoCo network
		mass_data.toArray((uint8_t*) &packet);
//		JetsonNetwork.send(&packet);
		portYIELD();
	} else {
		massSensorInstance = nullptr;
		MX_SPI1_Init();
		terminate();
		parent->resetProber();
	}
	osDelay(100);
}
