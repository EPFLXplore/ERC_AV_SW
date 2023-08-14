/*
 * ADS1234_thread.cpp
 *
 *  Created on: Apr 2, 2023
 *      Author: Yassine Bakkali, Vincent Nguyen
 */

#include <ADS1234_Mass_thread.hpp>
#include "Telemetry.h"

#include "Debugging/Debug.h"

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

// Declare your data with the proper data structure defined in DataStructures.h
static MassData mass_data;

// Declare the RoCo packet with the proper data structure defined in RoCo/Src/Protocol/Protocol23
static MassPacket mass_packet;

void ADS1234Thread::loop() {
	// Calibrate every 90 seconds
	ERROR_t err_ch1 = NoERROR;
	ERROR_t err_ch2 = NoERROR;
	ERROR_t err_ch3 = NoERROR;
	ERROR_t err_ch4 = NoERROR;
    if(xTaskGetTickCount()-start > 90000){
    	calibrating = true;
    	start = xTaskGetTickCount();
    	printf("Calibrating mass sensor... \n");
    }
#ifdef USE_LOW_PASS_FILTER
#ifdef CH1_ENABLE
    	err_ch1 = mass_sensor->get_units(AIN1, mass_data.mass[0], alpha, calibrating);
#endif
#ifdef CH2_ENABLE
    	err_ch2 = mass_sensor->get_units(AIN2, mass_data.mass[1], alpha, calibrating);
#endif
#ifdef CH3_ENABLE
    	err_ch3 = mass_sensor->get_units(AIN3, mass_data.mass[2], alpha, calibrating);
#endif
#ifdef CH4_ENABLE
    	err_ch4 = mass_sensor->get_units(AIN4, mass_data.mass[3], alpha, calibrating);
#endif
#elif
#ifdef CH1_ENABLE
    	err_ch1 = mass_sensor->get_units(AIN1, mass_data.mass[0], num_averages, calibrating);
#endif
#ifdef CH2_ENABLE
    	err_ch2 = mass_sensor->get_units(AIN2, mass_data.mass[1], num_averages, calibrating);
#endif
#ifdef CH3_ENABLE
    	err_ch3 = mass_sensor->get_units(AIN3, mass_data.mass[2], num_averages, calibrating);
#endif
#ifdef CH4_ENABLE
    	err_ch4 = mass_sensor->get_units(AIN4, mass_data.mass[3], num_averages, calibrating);
#endif
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

	if((err_ch1 == NoERROR) && (err_ch2 == NoERROR) && (err_ch3 == NoERROR) && (err_ch4 == NoERROR)) {
		if(monitor.enter(MASS_MONITOR)) {
			println("%s", mass_data.toString(cbuf));
		}

		mass_data.toArray((uint8_t*) &mass_packet);
		MAKE_IDENTIFIABLE(mass_packet);
		Telemetry::set_id(JETSON_NODE_ID);
		FDCAN1_network->send(&mass_packet);
		portYIELD();
	} else {
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
