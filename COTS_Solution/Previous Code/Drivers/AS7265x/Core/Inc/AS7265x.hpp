/*
 * AD7265x.hpp
 *
 *  Created on: 1 Apr 2023
 *      Author: Vincent
 */

#ifndef AS7265X_CORE_INC_AS7265X_HPP_
#define AS7265X_CORE_INC_AS7265X_HPP_

#include <stdbool.h>
#include <stdint.h>
#include "stm32h7xx_hal.h"

#include "AS7265x_defines.h"


class AS7265x {
public:
	AS7265x(I2C_HandleTypeDef* hi2c, uint8_t i2c_address);
	HAL_StatusTypeDef begin();
	HAL_StatusTypeDef isConnected();

	HAL_StatusTypeDef getDeviceType(uint8_t& val);
	HAL_StatusTypeDef getHardwareVersion(uint8_t& val);
	HAL_StatusTypeDef getMajorFirmwareVersion(uint8_t& val);
	HAL_StatusTypeDef getPatchFirmwareVersion(uint8_t& val);
	HAL_StatusTypeDef getBuildFirmwareVersion(uint8_t& val);

	HAL_StatusTypeDef getTemperature(uint8_t& val, uint8_t deviceNumber = 0); //Get temp in C of the master IC
	HAL_StatusTypeDef getTemperatureAverage(float& val);                    //Get average of all three ICs

	HAL_StatusTypeDef takeMeasurements();
	HAL_StatusTypeDef takeMeasurementsWithBulb();

	HAL_StatusTypeDef enableIndicator(); //Blue status LED
	HAL_StatusTypeDef disableIndicator();

	HAL_StatusTypeDef enableBulb(uint8_t device);
	HAL_StatusTypeDef disableBulb(uint8_t device);

	HAL_StatusTypeDef setGain(uint8_t gain);            //1 to 64x
	HAL_StatusTypeDef setMeasurementMode(uint8_t mode); //4 channel, other 4 channel, 6 chan, or 6 chan one shot
	HAL_StatusTypeDef setIntegrationCycles(uint8_t cycleValue);

	HAL_StatusTypeDef setBulbCurrent(uint8_t current, uint8_t device); //
	HAL_StatusTypeDef setIndicatorCurrent(uint8_t current);            //0 to 8mA

	HAL_StatusTypeDef enableInterrupt();
	HAL_StatusTypeDef disableInterrupt();

	HAL_StatusTypeDef softReset();

	HAL_StatusTypeDef dataAvailable(bool& available); //Returns true when data is available

	//Returns the various calibration data
	HAL_StatusTypeDef getCalibratedA(float& val);
	HAL_StatusTypeDef getCalibratedB(float& val);
	HAL_StatusTypeDef getCalibratedC(float& val);
	HAL_StatusTypeDef getCalibratedD(float& val);
	HAL_StatusTypeDef getCalibratedE(float& val);
	HAL_StatusTypeDef getCalibratedF(float& val);

	HAL_StatusTypeDef getCalibratedG(float& val);
	HAL_StatusTypeDef getCalibratedH(float& val);
	HAL_StatusTypeDef getCalibratedI(float& val);
	HAL_StatusTypeDef getCalibratedJ(float& val);
	HAL_StatusTypeDef getCalibratedK(float& val);
	HAL_StatusTypeDef getCalibratedL(float& val);

	HAL_StatusTypeDef getCalibratedR(float& val);
	HAL_StatusTypeDef getCalibratedS(float& val);
	HAL_StatusTypeDef getCalibratedT(float& val);
	HAL_StatusTypeDef getCalibratedU(float& val);
	HAL_StatusTypeDef getCalibratedV(float& val);
	HAL_StatusTypeDef getCalibratedW(float& val);

	//Get the various raw readings
	HAL_StatusTypeDef getA(uint16_t& val);
	HAL_StatusTypeDef getB(uint16_t& val);
	HAL_StatusTypeDef getC(uint16_t& val);
	HAL_StatusTypeDef getD(uint16_t& val);
	HAL_StatusTypeDef getE(uint16_t& val);
	HAL_StatusTypeDef getF(uint16_t& val);

	HAL_StatusTypeDef getG(uint16_t& val);
	HAL_StatusTypeDef getH(uint16_t& val);
	HAL_StatusTypeDef getI(uint16_t& val);
	HAL_StatusTypeDef getJ(uint16_t& val);
	HAL_StatusTypeDef getK(uint16_t& val);
	HAL_StatusTypeDef getL(uint16_t& val);

	HAL_StatusTypeDef getR(uint16_t& val);
	HAL_StatusTypeDef getS(uint16_t& val);
	HAL_StatusTypeDef getT(uint16_t& val);
	HAL_StatusTypeDef getU(uint16_t& val);
	HAL_StatusTypeDef getV(uint16_t& val);
	HAL_StatusTypeDef getW(uint16_t& val);

private:
	I2C_HandleTypeDef* hi2c;
	uint16_t i2c_address;
	HAL_StatusTypeDef getChannel(uint8_t channelRegister, uint8_t device, uint16_t& val);
	HAL_StatusTypeDef getCalibratedValue(uint8_t calAddress, uint8_t device, float& val);
	float convertBytesToFloat(uint32_t myLong);

	HAL_StatusTypeDef selectDevice(uint8_t device); //Change between the x51, x52, or x53 for data and settings

	HAL_StatusTypeDef virtualReadRegister(uint8_t virtualAddr, uint8_t& val);
	HAL_StatusTypeDef virtualWriteRegister(uint8_t virtualAddr, uint8_t dataToWrite);

	HAL_StatusTypeDef readRegister(uint8_t addr, uint8_t& val);
	HAL_StatusTypeDef writeRegister(uint8_t addr, uint8_t val);
	uint16_t maxWaitTime = 35; //Based on integration cycles
};


#endif /* AS7265X_CORE_INC_AS7265X_HPP_ */
