/*
 * AD7265x.cpp
 *
 *  Created on: 1 Apr 2023
 *      Author: Vincent
 */

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "stdio.h"
#include "Build.h"
#include <memory.h>
#include "AS7265x.hpp"

AS7265x::AS7265x(I2C_HandleTypeDef* hi2c_, uint8_t i2c_address_) :
									hi2c(hi2c_), i2c_address(i2c_address_ << 1)
{}

//Initializes the sensor with basic settings
//Returns false if sensor is not detected
HAL_StatusTypeDef AS7265x::begin() {
	HAL_StatusTypeDef status = isConnected();
	if (status != HAL_OK)
		return status;

	//Check to see if both slaves are detected
	uint8_t value;
	status = virtualReadRegister(AS7265X_DEV_SELECT_CONTROL, value);
	if (status != HAL_OK)
		return status;
	if ((value & 0b00110000) == 0)
		return HAL_ERROR; //Test if Slave1 and 2 are detected. If not, bail.

	status = setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_WHITE);
	if (status != HAL_OK)
		return status;

	status = setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_IR);
	if (status != HAL_OK)
		return status;

	status = setBulbCurrent(AS7265X_LED_CURRENT_LIMIT_12_5MA, AS7265x_LED_UV);
	if (status != HAL_OK)
		return status;

	status = disableBulb(AS7265x_LED_WHITE); //Turn off bulb to avoid heating sensor
	if (status != HAL_OK)
		return status;

	status = disableBulb(AS7265x_LED_IR);
	if (status != HAL_OK)
		return status;

	status = disableBulb(AS7265x_LED_UV);
	if (status != HAL_OK)
		return status;

	status = setIndicatorCurrent(AS7265X_INDICATOR_CURRENT_LIMIT_8MA); //Set to 8mA (maximum)
	if (status != HAL_OK)
		return status;

	status = enableIndicator();
	if (status != HAL_OK)
		return status;

	status = setIntegrationCycles(49); //50 * 2.8ms = 140ms. 0 to 255 is valid.
	if (status != HAL_OK)
		return status;
	//If you use Mode 2 or 3 (all the colors) then integration time is double. 140*2 = 280ms between readings.

	status = setGain(AS7265X_GAIN_64X); //Set gain to 64x
	if (status != HAL_OK)
		return status;

	status = setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT); //One-shot reading of VBGYOR
	if (status != HAL_OK)
		return status;

	status = enableInterrupt();
	if (status != HAL_OK)
		return status;

	return HAL_OK; //We're all setup!
}

HAL_StatusTypeDef AS7265x::getDeviceType(uint8_t& val) {
	return (virtualReadRegister(AS7265X_HW_VERSION_HIGH, val));
}

HAL_StatusTypeDef AS7265x::getHardwareVersion(uint8_t& val) {
	return (virtualReadRegister(AS7265X_HW_VERSION_LOW, val));
}

HAL_StatusTypeDef AS7265x::getMajorFirmwareVersion(uint8_t& val) {
	HAL_StatusTypeDef status;
	status = virtualWriteRegister(AS7265X_FW_VERSION_HIGH, 0x01); //Set to 0x01 for Major
	if (status != HAL_OK) {
		val = 0;
		return status;
	}
	status = virtualWriteRegister(AS7265X_FW_VERSION_LOW, 0x01);  //Set to 0x01 for Major
	if (status != HAL_OK) {
		val = 0;
		return status;
	}

    return (virtualReadRegister(AS7265X_FW_VERSION_LOW, val));
}

HAL_StatusTypeDef AS7265x::getPatchFirmwareVersion(uint8_t& val) {
	HAL_StatusTypeDef status;
	status = virtualWriteRegister(AS7265X_FW_VERSION_HIGH, 0x02); //Set to 0x02 for Patch
	status = virtualWriteRegister(AS7265X_FW_VERSION_HIGH, 0x01); //Set to 0x01 for Major
	if (status != HAL_OK) {
		val = 0;
		return status;
	}
	status = virtualWriteRegister(AS7265X_FW_VERSION_LOW, 0x02);  //Set to 0x02 for Patch
	status = virtualWriteRegister(AS7265X_FW_VERSION_HIGH, 0x01); //Set to 0x01 for Major
	if (status != HAL_OK) {
		val = 0;
		return status;
	}

	return (virtualReadRegister(AS7265X_FW_VERSION_LOW, val));
}

HAL_StatusTypeDef AS7265x::getBuildFirmwareVersion(uint8_t& val) {
	HAL_StatusTypeDef status;
	status = virtualWriteRegister(AS7265X_FW_VERSION_HIGH, 0x03); //Set to 0x03 for Build
	if (status != HAL_OK) {
		val = 0;
		return status;
	}
	status = virtualWriteRegister(AS7265X_FW_VERSION_LOW, 0x03);  //Set to 0x03 for Build
	if (status != HAL_OK) {
		val = 0;
		return status;
	}

	return (virtualReadRegister(AS7265X_FW_VERSION_LOW, val));
}

//Returns true if I2C device ack's
HAL_StatusTypeDef AS7265x::isConnected() {
  //Give IC 660ms for startup - max 1000ms
	for (uint8_t x = 0; x < 100; x++) {
		HAL_StatusTypeDef status;
#ifdef ENERGIA
		status = HAL_I2C_Master_Transmit(&hi2c, deviceAddress, 0x00, 1, 100)
#else
		status = HAL_I2C_IsDeviceReady(hi2c, i2c_address, 10, 100);
#endif

		if (status == HAL_OK)
			return status; // Sensor ACK'd
	}
	return HAL_TIMEOUT; // Sensor did not ACK
}

//Tells IC to take all channel measurements and polls for data ready flag
HAL_StatusTypeDef AS7265x::takeMeasurements() {
	HAL_StatusTypeDef status;
	status = setMeasurementMode(AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT); //Set mode to all 6-channels, one-shot
	if (status != HAL_OK)
		return status;
  //Wait for data to be ready
	int startTime = xTaskGetTickCount();
	bool available = false;
	while (available == false) { // will block the thread
		status = dataAvailable(available);
		if (status != HAL_OK)
			return status;

		if(xTaskGetTickCount() - startTime > (maxWaitTime)) {
			return HAL_ERROR; //Sensor failed to respond
		}

		taskYIELD();
		osDelay(AS7265X_POLLING_DELAY);
	}
	return HAL_OK;
	//Readings can now be accessed via getCalibratedA(), getJ(), etc
}

//Turns on all bulbs, takes measurements of all channels, turns off all bulbs
HAL_StatusTypeDef AS7265x::takeMeasurementsWithBulb() {
	HAL_StatusTypeDef status;
	status = enableBulb(AS7265x_LED_WHITE);
	if (status != HAL_OK)
		return status;

	status = enableBulb(AS7265x_LED_IR);
	if (status != HAL_OK)
		return status;

	status = enableBulb(AS7265x_LED_UV);
	if (status != HAL_OK)
		return status;

	status = takeMeasurements();
	if (status != HAL_OK)
		return status;

	status = disableBulb(AS7265x_LED_WHITE); //Turn off bulb to avoid heating sensor
	if (status != HAL_OK)
		return status;

	status = disableBulb(AS7265x_LED_IR);
	if (status != HAL_OK)
		return status;

	status = disableBulb(AS7265x_LED_UV);
	if (status != HAL_OK)
		return status;

	return HAL_OK;
}

//Get the various color readings
HAL_StatusTypeDef AS7265x::getG(uint16_t& val)
{
	return (getChannel(AS7265X_R_G_A, AS72652_VISIBLE, val));
}
HAL_StatusTypeDef AS7265x::getH(uint16_t& val)
{
	return (getChannel(AS7265X_S_H_B, AS72652_VISIBLE, val));
}
HAL_StatusTypeDef AS7265x::getI(uint16_t& val)
{
	return (getChannel(AS7265X_T_I_C, AS72652_VISIBLE, val));
}
HAL_StatusTypeDef AS7265x::getJ(uint16_t& val)
{
	return (getChannel(AS7265X_U_J_D, AS72652_VISIBLE, val));
}
HAL_StatusTypeDef AS7265x::getK(uint16_t& val)
{
	return (getChannel(AS7265X_V_K_E, AS72652_VISIBLE, val));
}
HAL_StatusTypeDef AS7265x::getL(uint16_t& val)
{
	return (getChannel(AS7265X_W_L_F, AS72652_VISIBLE, val));
}

//Get the various NIR readings
HAL_StatusTypeDef AS7265x::getR(uint16_t& val)
{
	return (getChannel(AS7265X_R_G_A, AS72651_NIR, val));
}
HAL_StatusTypeDef AS7265x::getS(uint16_t& val)
{
	return (getChannel(AS7265X_S_H_B, AS72651_NIR, val));
}
HAL_StatusTypeDef AS7265x::getT(uint16_t& val)
{
	return (getChannel(AS7265X_T_I_C, AS72651_NIR, val));
}
HAL_StatusTypeDef AS7265x::getU(uint16_t& val)
{
	return (getChannel(AS7265X_U_J_D, AS72651_NIR, val));
}
HAL_StatusTypeDef AS7265x::getV(uint16_t& val)
{
	return (getChannel(AS7265X_V_K_E, AS72651_NIR, val));
}
HAL_StatusTypeDef AS7265x::getW(uint16_t& val)
{
	return (getChannel(AS7265X_W_L_F, AS72651_NIR, val));
}

//Get the various UV readings
HAL_StatusTypeDef AS7265x::getA(uint16_t& val)
{
	return (getChannel(AS7265X_R_G_A, AS72653_UV, val));
}
HAL_StatusTypeDef AS7265x::getB(uint16_t& val)
{
	return (getChannel(AS7265X_S_H_B, AS72653_UV, val));
}
HAL_StatusTypeDef AS7265x::getC(uint16_t& val)
{
	return (getChannel(AS7265X_T_I_C, AS72653_UV, val));
}
HAL_StatusTypeDef AS7265x::getD(uint16_t& val)
{
	return (getChannel(AS7265X_U_J_D, AS72653_UV, val));
}
HAL_StatusTypeDef AS7265x::getE(uint16_t& val)
{
	return (getChannel(AS7265X_V_K_E, AS72653_UV, val));
}
HAL_StatusTypeDef AS7265x::getF(uint16_t& val)
{
	return (getChannel(AS7265X_W_L_F, AS72653_UV, val));
}

//A the 16-bit value stored in a given channel registerReturns
HAL_StatusTypeDef AS7265x::getChannel(uint8_t channelRegister, uint8_t device, uint16_t& val)
{
	HAL_StatusTypeDef status;
	status = selectDevice(device);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}

	uint8_t colorDataHigh;
  	status = virtualReadRegister(channelRegister, colorDataHigh); // High uint8_t
  	if (status != HAL_OK) {
  		val = 0;
  		return status;
  	}

  	uint8_t colorDataLow;
  	status = virtualReadRegister(channelRegister + 1, colorDataLow); // Low uint8_t
  	if (status != HAL_OK) {
  		val = 0;
  		return status;
  	}

  	val = (uint16_t)(colorDataHigh << 8) | colorDataLow;
  	return HAL_OK;
}

//Returns the various calibration data
HAL_StatusTypeDef AS7265x::getCalibratedA(float& val)
{
	return (getCalibratedValue(AS7265X_R_G_A_CAL, AS72653_UV, val));
}
HAL_StatusTypeDef AS7265x::getCalibratedB(float& val)
{
	return (getCalibratedValue(AS7265X_S_H_B_CAL, AS72653_UV, val));
}
HAL_StatusTypeDef AS7265x::getCalibratedC(float& val)
{
	return (getCalibratedValue(AS7265X_T_I_C_CAL, AS72653_UV, val));
}
HAL_StatusTypeDef AS7265x::getCalibratedD(float& val)
{
	return (getCalibratedValue(AS7265X_U_J_D_CAL, AS72653_UV, val));
}
HAL_StatusTypeDef AS7265x::getCalibratedE(float& val)
{
	return (getCalibratedValue(AS7265X_V_K_E_CAL, AS72653_UV, val));
}
HAL_StatusTypeDef AS7265x::getCalibratedF(float& val)
{
	return (getCalibratedValue(AS7265X_W_L_F_CAL, AS72653_UV, val));
}

//Returns the various calibration data
HAL_StatusTypeDef AS7265x::getCalibratedG(float& val)
{
	return (getCalibratedValue(AS7265X_R_G_A_CAL, AS72652_VISIBLE, val));
}
HAL_StatusTypeDef AS7265x::getCalibratedH(float& val)
{
	return (getCalibratedValue(AS7265X_S_H_B_CAL, AS72652_VISIBLE, val));
}
HAL_StatusTypeDef AS7265x::getCalibratedI(float& val)
{
	return (getCalibratedValue(AS7265X_T_I_C_CAL, AS72652_VISIBLE, val));
}
HAL_StatusTypeDef AS7265x::getCalibratedJ(float& val)
{
	return (getCalibratedValue(AS7265X_U_J_D_CAL, AS72652_VISIBLE, val));
}
HAL_StatusTypeDef AS7265x::getCalibratedK(float& val)
{
	return (getCalibratedValue(AS7265X_V_K_E_CAL, AS72652_VISIBLE, val));
}
HAL_StatusTypeDef AS7265x::getCalibratedL(float& val)
{
	return (getCalibratedValue(AS7265X_W_L_F_CAL, AS72652_VISIBLE, val));
}

HAL_StatusTypeDef AS7265x::getCalibratedR(float& val)
{
	return (getCalibratedValue(AS7265X_R_G_A_CAL, AS72651_NIR, val));
}
HAL_StatusTypeDef AS7265x::getCalibratedS(float& val)
{
	return (getCalibratedValue(AS7265X_S_H_B_CAL, AS72651_NIR, val));
}
HAL_StatusTypeDef AS7265x::getCalibratedT(float& val)
{
	return (getCalibratedValue(AS7265X_T_I_C_CAL, AS72651_NIR, val));
}
HAL_StatusTypeDef AS7265x::getCalibratedU(float& val)
{
	return (getCalibratedValue(AS7265X_U_J_D_CAL, AS72651_NIR, val));
}
HAL_StatusTypeDef AS7265x::getCalibratedV(float& val)
{
	return (getCalibratedValue(AS7265X_V_K_E_CAL, AS72651_NIR, val));
}
HAL_StatusTypeDef AS7265x::getCalibratedW(float& val)
{
	return (getCalibratedValue(AS7265X_W_L_F_CAL, AS72651_NIR, val));
}

//Given an address, read four bytes and return the floating point calibrated value
HAL_StatusTypeDef AS7265x::getCalibratedValue(uint8_t calAddress, uint8_t device, float& val) {
	HAL_StatusTypeDef status = selectDevice(device);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}

	uint8_t b0, b1, b2, b3;
	status = virtualReadRegister(calAddress + 0, b0);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}
	status = virtualReadRegister(calAddress + 1, b1);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}
  	status = virtualReadRegister(calAddress + 2, b2);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}
	status = virtualReadRegister(calAddress + 3, b3);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}

	//Channel calibrated values are stored big-endian
	uint32_t calBytes = 0;
	calBytes |= ((uint32_t)b0 << (8 * 3));
	calBytes |= ((uint32_t)b1 << (8 * 2));
	calBytes |= ((uint32_t)b2 << (8 * 1));
	calBytes |= ((uint32_t)b3 << (8 * 0));
	val = convertBytesToFloat(calBytes);
	return HAL_OK;
}

//Given 4 bytes returns the floating point value
float AS7265x::convertBytesToFloat(uint32_t myLong) {
	float myFloat;
	memcpy(&myFloat, &myLong, 4); //Copy bytes into a float
	return (myFloat);
}

//Mode 0: 4 channels out of 6 (see datasheet)
//Mode 1: Different 4 channels out of 6 (see datasheet)
//Mode 2: All 6 channels continuously
//Mode 3: One-shot reading of all channels
HAL_StatusTypeDef AS7265x::setMeasurementMode(uint8_t mode) {
	if (mode > 0b11)
		mode = 0b11; //Error check
	HAL_StatusTypeDef status;
	uint8_t value;
	//Read, mask/set, write
  	status = virtualReadRegister(AS7265X_CONFIG, value); //Read
  	if (status != HAL_OK)
  		return status;

  	value &= 0b11110011;                                 //Clear BANK bits
  	value |= (mode << 2);                                //Set BANK bits with user's choice
  	return virtualWriteRegister(AS7265X_CONFIG, value);  //Write
}

//Sets the gain value
//Gain 0: 1x (power-on default)
//Gain 1: 3.7x
//Gain 2: 16x
//Gain 3: 64x
HAL_StatusTypeDef AS7265x::setGain(uint8_t gain) {
	if (gain > 0b11)
		gain = 0b11;
	HAL_StatusTypeDef status;
	uint8_t value;
  //Read, mask/set, write
	status = virtualReadRegister(AS7265X_CONFIG, value); //Read
  	if (status != HAL_OK)
  		return status;

  	value &= 0b11001111;                                 //Clear GAIN bits
  	value |= (gain << 4);                                //Set GAIN bits with user's choice
  	return virtualWriteRegister(AS7265X_CONFIG, value);  //Write
}

//Sets the integration cycle amount
//Give this function a byte from 0 to 255.
//Time will be 2.8ms * [integration cycles + 1]
HAL_StatusTypeDef AS7265x::setIntegrationCycles(uint8_t cycleValue) {
	maxWaitTime = (int)(cycleValue * 2.8 * 3); //Wait for integration time + 70%
	return virtualWriteRegister(AS7265X_INTERGRATION_TIME, cycleValue); //Write
}

HAL_StatusTypeDef AS7265x::enableInterrupt() {
	HAL_StatusTypeDef status;
	uint8_t value;
	//Read, mask/set, write
  	status = virtualReadRegister(AS7265X_CONFIG, value); //Read
  	if (status != HAL_OK)
  		return status;
	value |= (1 << 6);                                   //Set INT bit
	return virtualWriteRegister(AS7265X_CONFIG, value);         //Write
}

//Disables the interrupt pin
HAL_StatusTypeDef AS7265x::disableInterrupt()
{
	HAL_StatusTypeDef status;
	uint8_t value;
	//Read, mask/set, write
  	status = virtualReadRegister(AS7265X_CONFIG, value); //Read
  	if (status != HAL_OK)
  		return status;
	value &= ~(1 << 6);                                  //Clear INT bit
	return virtualWriteRegister(AS7265X_CONFIG, value);         //Write
}

//Checks to see if DRDY flag is set in the control setup register
HAL_StatusTypeDef AS7265x::dataAvailable(bool& available) {
	HAL_StatusTypeDef status;
	uint8_t value;
	status = virtualReadRegister(AS7265X_CONFIG, value);
	if (status != HAL_OK) {
		available = false;
		return status;
	}
	available = (value & (1 << 1)); //Bit 1 is DATA_RDY
	return HAL_OK;
}

//Enable the LED or bulb on a given device
HAL_StatusTypeDef AS7265x::enableBulb(uint8_t device) {
	HAL_StatusTypeDef status;
	status = selectDevice(device);
	if (status != HAL_OK)
		return status;

	//Read, mask/set, write
	uint8_t value;
	status = virtualReadRegister(AS7265X_LED_CONFIG, value);
	if (status != HAL_OK)
		return status;

	value |= (1 << 3); //Set the bit
	return virtualWriteRegister(AS7265X_LED_CONFIG, value);
}

//Disable the LED or bulb on a given device
HAL_StatusTypeDef AS7265x::disableBulb(uint8_t device) {
	HAL_StatusTypeDef status;
	status = selectDevice(device);
	if (status != HAL_OK)
		return status;

	//Read, mask/set, write
	uint8_t value;
	status = virtualReadRegister(AS7265X_LED_CONFIG, value);
	if (status != HAL_OK)
		return status;

	value &= ~(1 << 3); //Clear the bit
	return virtualWriteRegister(AS7265X_LED_CONFIG, value);
}

//Set the current limit of bulb/LED.
//Current 0: 12.5mA
//Current 1: 25mA
//Current 2: 50mA
//Current 3: 100mA
HAL_StatusTypeDef AS7265x::setBulbCurrent(uint8_t current, uint8_t device) {
	HAL_StatusTypeDef status;
	status = selectDevice(device);
	if (status != HAL_OK)
		return status;

	// set the current
	if (current > 0b11)
		current = 0b11;                                        	//Limit to two bits
	uint8_t value;
  	status = virtualReadRegister(AS7265X_LED_CONFIG, value); 	//Read
	if (status != HAL_OK)
		return status;
	value &= 0b11001111;                                     	//Clear ICL_DRV bits
	value |= (current << 4);                                 	//Set ICL_DRV bits with user's choice
	return virtualWriteRegister(AS7265X_LED_CONFIG, value);     //Write
}

//As we read various registers we have to point at the master or first/second slave
HAL_StatusTypeDef AS7265x::selectDevice(uint8_t device) {
  //Set the bits 0:1. Just overwrite whatever is there because masking in the correct value doesn't work.
  return virtualWriteRegister(AS7265X_DEV_SELECT_CONTROL, device);

  //This fails
  //HAL_StatusTypeDef status;
  //uint8_t value;
  //status = virtualReadRegister(AS7265X_DEV_SELECT_CONTROL, value);
  // if (status != HAL_OK)
  //	return status;
  //value &= 0b11111100; //Clear lower two bits
  //if(device < 3) value |= device; //Set the bits
  //return virtualWriteRegister(AS7265X_DEV_SELECT_CONTROL, value);
}

//Enable the onboard indicator LED
HAL_StatusTypeDef AS7265x::enableIndicator() {
	HAL_StatusTypeDef status;
	status = selectDevice(AS72651_NIR);
	if (status != HAL_OK)
		return status;

	//Read, mask/set, write
	uint8_t value;
  	status = virtualReadRegister(AS7265X_LED_CONFIG, value);
	if (status != HAL_OK)
		return status;
	value |= (1 << 0); //Set the bit

	return virtualWriteRegister(AS7265X_LED_CONFIG, value);
}

//Disable the onboard indicator LED
HAL_StatusTypeDef AS7265x::disableIndicator() {
	HAL_StatusTypeDef status;
	status = selectDevice(AS72651_NIR);
	if (status != HAL_OK)
		return status;

  //Read, mask/set, write
	uint8_t value;
  	status = virtualReadRegister(AS7265X_LED_CONFIG, value);
	if (status != HAL_OK)
		return status;
	value &= ~(1 << 0); //Clear the bit

	return virtualWriteRegister(AS7265X_LED_CONFIG, value);
}

//Set the current limit of onboard LED. Default is max 8mA = 0b11.
HAL_StatusTypeDef AS7265x::setIndicatorCurrent(uint8_t current) {
	HAL_StatusTypeDef status;
	status = selectDevice(AS72651_NIR);
	if (status != HAL_OK)
		return status;

	if (current > 0b11)
		current = 0b11;
	//Read, mask/set, write
	uint8_t value;
 	status = virtualReadRegister(AS7265X_LED_CONFIG, value); //Read
	if (status != HAL_OK)
		return status;
	value &= 0b11111001;                                     //Clear ICL_IND bits
	value |= (current << 1);                                 //Set ICL_IND bits with user's choice

	return virtualWriteRegister(AS7265X_LED_CONFIG, value); //Write
}

//Returns the temperature of a given device in C
HAL_StatusTypeDef AS7265x::getTemperature(uint8_t& val, uint8_t deviceNumber) {
	HAL_StatusTypeDef status;
	status = selectDevice(deviceNumber);
	if (status != HAL_OK)
		return status;

	return (virtualReadRegister(AS7265X_DEVICE_TEMP, val));
}

//Returns an average of all the sensor temps in C
HAL_StatusTypeDef AS7265x::getTemperatureAverage(float& val) {
	HAL_StatusTypeDef status;
	long sum = 0;
	uint8_t res = 0;

	for (uint8_t i = 0; i < 3; i++) {
		status = getTemperature(res, i);
		if (status != HAL_OK)
			return status;
		sum += res;
	}
	val = (float)sum / 3.;
  	return HAL_OK;
}

//Does a soft reset
//Give sensor at least 1000ms to reset
HAL_StatusTypeDef AS7265x::softReset() {
	HAL_StatusTypeDef status;
	//Read, mask/set, write
	uint8_t value;
  	status = virtualReadRegister(AS7265X_CONFIG, value); //Read
  	if (status != HAL_OK)
  		return status;
  	value |= (1 << 7);                                   //Set RST bit, automatically cleared after reset
  	return virtualWriteRegister(AS7265X_CONFIG, value);  //Write
}

//Read a virtual register from the AS7265x
HAL_StatusTypeDef AS7265x::virtualReadRegister(uint8_t virtualAddr, uint8_t& val)
{
	HAL_StatusTypeDef status;
	uint8_t status_reg;

	//Do a prelim check of the read register
	status = readRegister(AS7265X_STATUS_REG, status_reg);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}

	uint8_t dummy;
	if ((status_reg & AS7265X_RX_VALID) != 0) //There is data to be read
		status = readRegister(AS7265X_READ_REG, dummy); //Read the byte but do nothing with it
	if (status != HAL_OK) {
		val = 0;
		return status;
	}

	//Wait for WRITE flag to clear
	int startTime = xTaskGetTickCount();
	while (1) {
		if(xTaskGetTickCount() - startTime > maxWaitTime) {
			val = 0;
			return HAL_ERROR; //Sensor failed to respond
		}

		status = readRegister(AS7265X_STATUS_REG, status_reg);
		if (status != HAL_OK) {
			val = 0;
			return status;
		}

		if ((status_reg & AS7265X_TX_VALID) == 0)
			break; // If TX bit is clear, it is ok to write

		taskYIELD();
		osDelay(AS7265X_POLLING_DELAY);
	}

	// Send the virtual register address (bit 7 should be 0 to indicate we are reading a register).
	status = writeRegister(AS7265X_WRITE_REG, virtualAddr);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}

	//Wait for READ flag to be set
	startTime = xTaskGetTickCount();
	while (1){
		volatile int count = xTaskGetTickCount();
		if(count - startTime > maxWaitTime) {
			val = 0;
			return HAL_ERROR; //Sensor failed to respond
		}

		status = readRegister(AS7265X_STATUS_REG, status_reg);
		if (status != HAL_OK) {
			val = 0;
			return status;
		}

		if ((status_reg & AS7265X_RX_VALID) != 0)
			break; // Read data is ready.

		taskYIELD();
		osDelay(AS7265X_POLLING_DELAY);
	}
	uint8_t incoming;
  	status = readRegister(AS7265X_READ_REG, incoming);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}

  	val = incoming;
  	return HAL_OK;
}


//Write to a virtual register in the AS726x
HAL_StatusTypeDef AS7265x::virtualWriteRegister(uint8_t virtualAddr, uint8_t dataToWrite) {
	HAL_StatusTypeDef status;
	uint8_t status_reg;

	//Wait for WRITE register to be empty
	unsigned long startTime = xTaskGetTickCount();
	while (1) {
		if(xTaskGetTickCount() - startTime > maxWaitTime)
			return HAL_ERROR; //Sensor failed to respond

		status = readRegister(AS7265X_STATUS_REG, status_reg);
		if (status != HAL_OK)
			return status;

		if ((status_reg & AS7265X_TX_VALID) == 0)
			break; // No inbound TX pending at slave. Okay to write now.

		taskYIELD();
		osDelay(AS7265X_POLLING_DELAY);
	}

	// Send the virtual register address (setting bit 7 to indicate we are writing to a register).
	status = writeRegister(AS7265X_WRITE_REG, (virtualAddr | 1 << 7));
	if (status != HAL_OK)
		return status;

	//Wait for WRITE register to be empty
	startTime = xTaskGetTickCount();
	while (1) {
		volatile int count = xTaskGetTickCount();
		if(count - startTime > maxWaitTime)
			return HAL_ERROR; //Sensor failed to respond
		status = readRegister(AS7265X_STATUS_REG, status_reg);
		if (status != HAL_OK)
			return status;

		if ((status & AS7265X_TX_VALID) == 0)
			break; // No inbound TX pending at slave. Okay to write now.

		taskYIELD();
		osDelay(AS7265X_POLLING_DELAY);
  }

  // Send the data to complete the operation.
  return writeRegister(AS7265X_WRITE_REG, dataToWrite);
}


HAL_StatusTypeDef AS7265x::readRegister(uint8_t addr, uint8_t& val) {
	HAL_StatusTypeDef status;
	uint8_t data = 0;

	status = HAL_I2C_Mem_Read(hi2c, i2c_address, addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
	if (status != HAL_OK) {
		val = 0;
		return status;
	}
	val = data;
	return HAL_OK;
}

HAL_StatusTypeDef AS7265x::writeRegister(uint8_t addr, uint8_t value) {
	HAL_StatusTypeDef status;

	status = HAL_I2C_IsDeviceReady(hi2c, i2c_address, 10, 100);
	if (status != HAL_OK)
		return status;

	return HAL_I2C_Mem_Write(hi2c, i2c_address, addr, I2C_MEMADD_SIZE_8BIT, &value, 1, 10);
}

