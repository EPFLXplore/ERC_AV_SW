/*
 * lis3mdl_sens.c
 *
 *  Created on: Mar 23, 2023
 *      Author: leo
 */

#include <bmi088_sens.hpp>
#include <lis3mdl_sens.hpp>
#include <cassert>
#include "Thread.h"
#include <chrono>

bool LIS3MDL_Sens::write_reg(uint8_t reg_addr, uint8_t* data, uint16_t len){
	int8_t rslt = 0;

	vTaskSuspendAll();

	rslt = HAL_I2C_Mem_Write(this->i2c, this->dev_addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, I2C_TIMEOUT);

	xTaskResumeAll();

	return rslt == 0;
}

bool LIS3MDL_Sens::read_reg(uint8_t reg_addr, uint8_t* data, uint16_t len){
	int8_t rslt = 0;

	vTaskSuspendAll();

	rslt = HAL_I2C_Mem_Read(this->i2c, this->dev_addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, HAL_MAX_DELAY);

	xTaskResumeAll();

	return rslt == 0;

}

void LIS3MDL_Sens::set_config(LIS3MDL_Sens::Config conf){
	this->config = conf;
}

bool LIS3MDL_Sens::init(){

	//this->mag_bias_offset[0] = 1.45536281;
	//this->mag_bias_offset[1] = 1.38531835;
	//this->mag_bias_offset[2] = -0.06510749;

	//this->mag_transformation_matrix = std::vector<std::vector<float>>{
	//	{0.924367, 0.0629682, -0.0197413},
	//	{0.0629682, 0.943557, -0.00820933},
	//	{-0.0197413, -0.00820933, 0.843738}
	//};

	this->mag_bias_offset[0] = 1.44211777;
	this->mag_bias_offset[1] =  1.38626137;
	this->mag_bias_offset[2] = -0.04207021;
	//1.41601624  1.40244085 -0.02389689
	//3-0.0727265  -0.07886059 -0.05524782

//	this->mag_transformation_matrix = std::vector<std::vector<float>>
//			{{ 0.9071028  , 0.06364516 , -0.02251928},
//		 { 0.06364516 , 0.95158869 , -0.00904196},
//		 {-0.02251928 ,-0.00904196 , 0.86997782}
//};
	this->mag_transformation_matrix = std::vector<std::vector<float>>
			//{{0.9348139732187145, 0.058537637989461634, -0.013028710056905729}, {0.05853763798946159, 0.9411224654780331, -0.0203816213348695}, {-0.01302871005690573, -0.020381621334869506, 0.8342913962266072}};
	{{0.9258445110192737, 0.06241061676482877, -0.009587443898741473}, {0.062410616764828716, 0.9473810553960268, 0.004452118354963165}, {-0.0095874438987415, 0.004452118354963147, 0.8581392697071786}};

	if(this->chip_id_ok() == false){
		return false;
	}

	if(this->set_odr(this->config.odr_config) == false){
		return false;
	}
	if(this->set_z_odr(this->config.odr_config) == false){
		return false;
	}

	if(this->set_range(this->config.range_config) == false){
		return false;
	}

	//if(this->set_soft_reset(this->config.soft_reset) == false){
	//	return false;
	//}

	if(this->set_self_test(this->config.self_test_enable) == false){
		return false;
	}


	if(this->set_cont_conv_mode(this->config.cont_conv_mode) == false){
		return false;
	}

	//if(this->set_fast_read(this->config.fast_read) == false){
	//	return false;
	//}

	return true;
}

bool LIS3MDL_Sens::chip_id_ok(){
 uint8_t to_read = 0;
 assert(read_reg(LIS3_WHO_AM_I, &to_read, 1));
 return LIS3_CHIP_ID == to_read;
}

bool LIS3MDL_Sens::set_odr(const LIS3MDL_Sens::LIS3MDL_Odr& odr){

switch(odr){
	//Table 20 in datasheet
case LIS3MDL_Sens::LIS3MDL_Odr::Odr_1000Hz:{
	uint8_t value = 0b00000010;
	value = 0 | (1<<1);
	assert(this->write_reg(LIS3_CTRL_REG1, &value, 1) == true);
	osDelay(50);
	uint8_t read_val = 0;
	assert(this->read_reg(LIS3_CTRL_REG1, &read_val, 1) == true);
	if(this->set_z_odr(odr) == false){
		return false;
	}
	return value == read_val;
}
case LIS3MDL_Sens::LIS3MDL_Odr::Odr_560Hz:{
	uint8_t value = 0b00100010;
	assert(this->write_reg(LIS3_CTRL_REG1, &value, 1) == true);
	osDelay(50);
	uint8_t read_val = 0;
	assert(this->read_reg(LIS3_CTRL_REG1, &read_val, 1) == true);
	if(this->set_z_odr(odr) == false){
			return false;
		}
	return value == read_val;
}
case LIS3MDL_Sens::LIS3MDL_Odr::Odr_300Hz:{
	uint8_t value = 0b01000010;
	assert(this->write_reg(LIS3_CTRL_REG1, &value, 1) == true);
	osDelay(50);
	uint8_t read_val = 0;
	assert(this->read_reg(LIS3_CTRL_REG1, &read_val, 1) == true);
	if(this->set_z_odr(odr) == false){
			return false;
		}
	return value == read_val;
}
case LIS3MDL_Sens::LIS3MDL_Odr::Odr_155Hz:{
	uint8_t value = 0b01100010;
	assert(this->write_reg(LIS3_CTRL_REG1, &value, 1) == true);
	osDelay(50);
	uint8_t read_val = 0;
	assert(this->read_reg(LIS3_CTRL_REG1, &read_val, 1) == true);
	if(this->set_z_odr(odr) == false){
			return false;
		}
	return value == read_val;
}
}

	return false;
}

bool LIS3MDL_Sens::set_self_test(const bool& self_test){
	uint8_t to_write = 0;
	assert(this->read_reg(LIS3_CTRL_REG1, &to_write, 1));
	if(self_test == true){
		to_write = to_write | 0b00000001;
		assert(this->write_reg(LIS3_CTRL_REG1, &to_write, 1));
		osDelay(50);
		uint8_t to_read = 0;
		assert(this->read_reg(LIS3_CTRL_REG1, &to_read, 1));
		return to_read == to_write;
	}
	else{
		to_write = to_write & 0b11111110;
		assert(this->write_reg(LIS3_CTRL_REG1, &to_write, 1));
		osDelay(50);
		uint8_t to_read = 0;
		assert(this->read_reg(LIS3_CTRL_REG1, &to_read, 1));
		return to_read == to_write;
	}
}

bool LIS3MDL_Sens::set_range(const LIS3MDL_Sens::LIS3MDL_Range& range){
	switch(range){
	case LIS3MDL_Sens::LIS3MDL_Range::Range_4:{
		uint8_t to_write = 0x0;
		assert(this->read_reg(LIS3_CTRL_REG2, &to_write, 1));
		//to_write = to_write & 0b10011111;
		assert(this->write_reg(LIS3_CTRL_REG2, &to_write, 1));
		osDelay(50);
		uint8_t to_read = 0;
		assert(this->read_reg(LIS3_CTRL_REG2, &to_read, 1));
		this->mag_scale_real = 4.0;
		return to_write == to_read;
	}
	case LIS3MDL_Sens::LIS3MDL_Range::Range_8:{
		uint8_t to_write = 0x0;
		assert(this->read_reg(LIS3_CTRL_REG2, &to_write, 1));
		to_write = 0b00100000;
		assert(this->write_reg(LIS3_CTRL_REG2, &to_write, 1));
		osDelay(50);
		uint8_t to_read = 0;
		assert(this->read_reg(LIS3_CTRL_REG2, &to_read, 1));
		this->mag_scale_real = 8.0f;
		return to_write == to_read;
	}
	case LIS3MDL_Sens::LIS3MDL_Range::Range_12:{
		uint8_t to_write = 0x0;
		assert(this->read_reg(LIS3_CTRL_REG2, &to_write, 1));
		to_write = 0b01000000;
		assert(this->write_reg(LIS3_CTRL_REG2, &to_write, 1));
		osDelay(50);
		uint8_t to_read = 0;
		assert(this->read_reg(LIS3_CTRL_REG2, &to_read, 1));
		this->mag_scale_real = 12.0;
		return to_write == to_read;
	}
	case LIS3MDL_Sens::LIS3MDL_Range::Range_16:{
		uint8_t to_write = 0x0;
		assert(this->read_reg(LIS3_CTRL_REG2, &to_write, 1));
		to_write = 0b01100000;
		assert(this->write_reg(LIS3_CTRL_REG2, &to_write, 1));
		osDelay(50);
		uint8_t to_read = 0;
		assert(this->read_reg(LIS3_CTRL_REG2, &to_read, 1));
		this->mag_scale_real = 16.0;
		return to_write == to_read;
	}

	}
	return false;
}

bool LIS3MDL_Sens::set_reboot(const bool& reboot){
	uint8_t to_write = 0x0;
	assert(this->read_reg(LIS3_CTRL_REG2, &to_write, 1));
	if(reboot == true){
		to_write = to_write | 0b00001000;
		assert(this->write_reg(LIS3_CTRL_REG2, &to_write, 1));
		osDelay(50);
		uint8_t to_read = 0;
		assert(this->read_reg(LIS3_CTRL_REG2, &to_read, 1));
		return to_write = to_read;
	}
	else{
		to_write = to_write & ~(0b00001000);
		assert(this->write_reg(LIS3_CTRL_REG2, &to_write, 1));
		osDelay(50);
		uint8_t to_read = 0;
		assert(this->read_reg(LIS3_CTRL_REG2, &to_read, 1));
		return to_write = to_read;
	}
}

bool LIS3MDL_Sens::set_soft_reset(const bool& soft_reset){
	uint8_t to_write = 0x0;
	assert(this->read_reg(LIS3_CTRL_REG2, &to_write, 1));
	if(soft_reset == true){
		to_write = to_write | 0b00000100;
		assert(this->write_reg(LIS3_CTRL_REG2, &to_write, 1));
		osDelay(50);
		uint8_t to_read = 0;
		assert(this->read_reg(LIS3_CTRL_REG2, &to_read, 1));
		return to_read == to_write;
	}
	else{
		to_write = to_write & ~(0b00000100);
		assert(this->write_reg(LIS3_CTRL_REG2, &to_write, 1));
		osDelay(50);
		uint8_t to_read = 0x0;
		assert(this->read_reg(LIS3_CTRL_REG2, &to_read, 1));
		return to_read == to_write;
	}
}

bool LIS3MDL_Sens::set_cont_conv_mode(const bool& cont_conv){
	if(cont_conv == true){
		uint8_t to_write = 0x0;
		assert(this->write_reg(LIS3_CTRL_REG3, &to_write, 1));
		osDelay(50);
		uint8_t to_read = 0;
		assert(this->read_reg(LIS3_CTRL_REG3, &to_read, 1));
		return to_read == to_write;
	}
	else{
		uint8_t to_write = 0b00000011;
		assert(this->write_reg(LIS3_CTRL_REG3, &to_write, 1));
		osDelay(50);
		uint8_t to_read = 0;
		assert(this->read_reg(LIS3_CTRL_REG3, &to_read, 1));
		return to_read == to_write;
	}
}

bool LIS3MDL_Sens::set_z_odr(const LIS3MDL_Sens::LIS3MDL_Odr& odr){
	switch(odr){
	case LIS3MDL_Sens::LIS3MDL_Odr::Odr_1000Hz:{
		uint8_t to_write = 0x0;
		assert(this->write_reg(LIS3_CTRL_REG4, &to_write, 1));
		osDelay(50);
		uint8_t to_read = 0x0;
		assert(this->read_reg(LIS3_CTRL_REG4, &to_read, 1));
		return to_read == to_write;
	}
	case LIS3MDL_Sens::LIS3MDL_Odr::Odr_560Hz:{
		uint8_t to_write = 0b00000100;
		assert(this->write_reg(LIS3_CTRL_REG4, &to_write, 1));
		osDelay(50);
		uint8_t to_read = 0x0;
		assert(this->read_reg(LIS3_CTRL_REG4, &to_read, 1));
		return to_read == to_write;
	}
	case LIS3MDL_Sens::LIS3MDL_Odr::Odr_300Hz:{
		uint8_t to_write = 0b00001000;
		assert(this->write_reg(LIS3_CTRL_REG4, &to_write, 1));
		osDelay(50);
		uint8_t to_read = 0x0;
		assert(this->read_reg(LIS3_CTRL_REG4, &to_read, 1));
		return to_read == to_write;
	}
	case LIS3MDL_Sens::LIS3MDL_Odr::Odr_155Hz:{
		uint8_t to_write = 0b00001100;
		assert(this->write_reg(LIS3_CTRL_REG4, &to_write, 1));
		osDelay(50);
		uint8_t to_read = 0x0;
		assert(this->read_reg(LIS3_CTRL_REG4, &to_read, 1));
		return to_read == to_write;
	}
	}
	return false;
}


void LIS3MDL_Sens::get_data(){
	uint8_t buff[6];
	int16_t mval[3];
	assert(read_reg((LIS3_OUT_X_L | 0x80), buff, 6));
	//assert(read_reg(LIS3_OUT_X_L, buff, 6));

	mval[0] = (int16_t)(buff[1]<<8 | buff[0]);
	mval[1] = (int16_t)(buff[3]<<8 | buff[2]);
	mval[2] = (int16_t)(buff[5]<<8 | buff[4]);
	this->mag_val[0] = mval[0]*this->mag_scale_real/32768.0f;
	this->mag_val[1] = mval[1]*this->mag_scale_real/32768.0f;
	this->mag_val[2] = mval[2]*this->mag_scale_real/32768.0f;
}



LIS3MDL_Sens::xyz LIS3MDL_Sens::get_last_mag(){
	this->get_data();
	return LIS3MDL_Sens::xyz{this->mag_val[0], this->mag_val[1], this->mag_val[2]};
}

void LIS3MDL_Sens::matrix_vec_multiply(const std::vector<std::vector<float>>& matrix, const float* vector, float* result){

	for(int x = 0; x<3; x++){
	    	result[x] = matrix[x][0] * vector[0] + matrix[x][1]*vector[1] + matrix[x][2]*vector[2];
	}
}

LIS3MDL_Sens::xyz LIS3MDL_Sens::get_last_mag_transformed(){
	this->get_data();
	float temp[3];
	for(int i = 0; i<3; i++){
		temp[i] = this->mag_val[i] - this->mag_bias_offset[i];
	}
	float to_ret[3];
	this->matrix_vec_multiply(this->mag_transformation_matrix, temp, to_ret);
	return LIS3MDL_Sens::xyz{
		to_ret[0], to_ret[1], to_ret[2]
	};

}


//bool lis3mdl_check_alive(struct LIS3MDL_Sens* sens){
//	sens->dev_addr = LIS3_I2C_ADDR;
//	uint8_t to_read = 0;
//	uint8_t whoami_read_res = read_reg(sens->i2c, sens->dev_addr, LIS3_WHO_AM_I, &to_read, 1);
//	if(to_read == LIS3_CHIP_ID){
//		return true;
//	}
//	else{
//		return false;
//	}
//}
//
//
//bool lis3mdl_config_odr(struct LIS3MDL_Sens* sens, enum LIS3MDL_ODR_CONF odr){
//	uint8_t value = 0;
//	switch(odr){
//	case LIS3MDL_ODR_1000:{
//		//if 1000hz, then, in order, st = 1/0, FAST_ODR = 11, DO = 100 (default, since
//		//if FAST_ODR is specified, DO is ignored, and OM is UHP (11)
//		//uint8_t config_reg = 0b11110000; //st = 1
//		uint8_t config_reg = 0b01110000; //st=0
//		uint8_t conf_reg_write_res = write_reg(sens->i2c, sens->dev_addr, LIS3_CTRL_REG1, &config_reg, 1);
//		if(conf_reg_write_res != 0){
//			return false;
//		}
//	}
//	default:{
////		return false;
//	}
//	}
//	//set the gauss scale
//	uint8_t scale_to_write = 0x0;
//	uint8_t conf_reg_write_res = write_reg(sens->i2c, sens->dev_addr, LIS3_CTRL_REG2, &scale_to_write, 1);
//
//	//set the conversion mode
//	uint8_t cont_mode_to_write = 0x00;
//	conf_reg_write_res = write_reg(sens->i2c, sens->dev_addr, LIS3_CTRL_REG3, &cont_mode_to_write, 1);
//
//	//set the power mode on z axis
//	uint8_t z_axis_mode_to_write = 0x0C;
//	conf_reg_write_res = write_reg(sens->i2c, sens->dev_addr, LIS3_CTRL_REG4, &z_axis_mode_to_write, 1);
//
//	return true;
//}
//
//
//void lis3mdl_get_data(struct LIS3MDL_Sens* sens){
//	uint8_t buff[6];
//	uint8_t read_data_res = read_reg(sens->i2c, sens->dev_addr, (LIS3_OUT_X_L | 0x80), sens->mag_val, 6);
//}
//
//
//
//
//
//
//
//
//
