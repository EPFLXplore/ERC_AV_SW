#include <stdio.h>
#include "bmi08x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32h7xx_hal.h"
#include "i2c.h"
#include "bmi08_defs.h"
#include "bmi08.h"
#include "cmsis_os.h"
#include <bmi088_sens.hpp>
#include <bmi088_sens_defs.hpp>
#include <cassert>
#include <stdio.h>
#include <chrono>




bool BMI088_Sens::write_reg(uint8_t reg_addr, uint8_t* data, uint16_t len, uint8_t d_addr){
	int8_t rslt = 0;

	vTaskSuspendAll();

	rslt = HAL_I2C_Mem_Write(this->i2c, d_addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, HAL_MAX_DELAY);

	xTaskResumeAll();

	return rslt == 0;
}

bool BMI088_Sens::read_reg(uint8_t reg_addr, uint8_t* data, uint16_t len, uint8_t d_addr){
	int8_t rslt = 0;

	vTaskSuspendAll();

	rslt = HAL_I2C_Mem_Read(this->i2c, d_addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, HAL_MAX_DELAY);

	xTaskResumeAll();

	return rslt == 0;

}

void BMI088_Sens::set_config(const BMI088_Sens::Config& conf){
	this->config = conf;
	return;
}

bool BMI088_Sens::initialize_accel(){
	this->tr_x[0] = 1.0f; this->tr_x[1] = 0.0f; this->tr_x[2] = 0.0f;
	this->tr_y[0] = 0.0f; this->tr_y[1] = 1.0f; this->tr_y[2] = 0.0f;
	this->tr_z[0] = 0.0f; this->tr_z[1] = 0.0f; this->tr_z[2] = 1.0f;

	this->bias_offset_acc.x = 0.0036275283943211357;
	this->bias_offset_acc.y = -0.00010062347530493902;
	this->bias_offset_acc.z = -0.00283879874025195;

	this->bias_offset_gyro.x = 0.0036176446355364457;
	this->bias_offset_gyro.y = -0.00018707559244075588;
	this->bias_offset_gyro.z = -0.0028493075692430756;

  if(this->chip_id_ok_acc() == false){
    return false;
  }
  osDelay(100);
  this->soft_reset_acc();
  osDelay(100);
  
  if(this->set_power_acc(true) == false){
    return false;
  }
  osDelay(100);

  if(this->set_mode_acc(true) == false){
    return false;
  }

  osDelay(100);

  if(this->self_test_acc() == false){
    return false;
  }
  osDelay(100);
  //this->soft_reset_acc();
  osDelay(100);

  if(this->set_power_acc(true) == false){
     return false;
   }
   osDelay(100);

   if(this->set_mode_acc(true) == false){
     return false;
   }
   osDelay(100);

  if(this->set_range_acc(this->config.range_acc_conf) == false){
    return false;
  }

  osDelay(100);

  if(this->set_odr_acc(this->config.odr_conf) == false){
    return false;
  }

  osDelay(100);

  return true;

}

bool BMI088_Sens::initialize_gyro(){
	this->tr_x[0] = 1.0f; this->tr_x[1] = 0.0f; this->tr_x[2] = 0.0f;
	this->tr_y[0] = 0.0f; this->tr_y[1] = 1.0f; this->tr_y[2] = 0.0f;
	this->tr_z[0] = 0.0f; this->tr_z[1] = 0.0f; this->tr_z[2] = 1.0f;

	if(this->chip_id_ok_gyro() == false){
		return false;
	}

	//this->soft_reset_gyro();

	if(this->set_range_gyro(this->config.range_gyro_conf) == false){
		return false;
	}

	if(this->set_drdy_int_gyro(true) == false){
		return false;
	}

	if(this->set_odr_gyro(this->config.odr_gyro_conf) == false){
		return false;
	}
}

bool BMI088_Sens::chip_id_ok_gyro(){
	uint8_t chip_id = 0;
	assert(read_reg(GYRO_CHIP_ID_ADDR, &chip_id, 1, this->gyro_dev_addr) == true);
	return (GET_FIELD(GYRO_CHIP_ID, chip_id) == GYRO_CHIP_ID)? true:false;
	//return chip_id == this->gyro_dev_addr;

}

bool BMI088_Sens::chip_id_ok_acc(){
  uint8_t chip_id = 0;
  assert(read_reg(BMI08_REG_ACCEL_CHIP_ID, &chip_id, 1, this->acc_dev_addr) == true);
  return chip_id == ACC_CHIP_ID;
}

bool BMI088_Sens::soft_reset_acc(){
  uint8_t to_write_soft_reset = 0;
  to_write_soft_reset = SET_FIELD(to_write_soft_reset, ACC_SOFT_RESET, ACC_RESET_CMD);
  bool write_res = false;
  write_res = write_reg(ACC_SOFT_RESET_ADDR, &to_write_soft_reset, 1, this->acc_dev_addr);
  osDelay(250);
  uint8_t to_read_soft_reset = 0;
  to_read_soft_reset = GET_FIELD(ACC_SOFT_RESET, ACC_RESET_CMD);
  assert(read_reg(ACC_SOFT_RESET_ADDR, &to_read_soft_reset, 1, this->acc_dev_addr) == true);

  return to_read_soft_reset == to_write_soft_reset;
}


bool BMI088_Sens::set_range_acc(const BMI088_Sens::Range_acc& range){
  uint8_t to_read = 0;
  uint8_t to_write = 0;
  switch(range){
    case BMI088_Sens::Range_acc::Mg3:
      to_write = ACC_RANGE_3G;
      this->range_integer = 3;
      assert(read_reg( ACC_RANGE_ADDR, &to_read, 1, this->acc_dev_addr));
      to_write = SET_FIELD(to_read, ACC_RANGE, ACC_RANGE_3G);
      assert(write_reg( ACC_RANGE_ADDR, &to_write, 1, this->acc_dev_addr));
      break;
    case BMI088_Sens::Range_acc::Mg6:
      to_write = ACC_RANGE_6G;
      this->range_integer = 6;
      assert(read_reg( ACC_RANGE_ADDR, &to_read, 1, this->acc_dev_addr));
      to_write = SET_FIELD(to_read, ACC_RANGE, ACC_RANGE_6G);
      assert(write_reg( ACC_RANGE_ADDR, &to_write, 1, this->acc_dev_addr));
      break;
    case BMI088_Sens::Range_acc::Mg12:
      to_write = ACC_RANGE_12G;
      this->range_integer = 12;
      assert(read_reg( ACC_RANGE_ADDR, &to_read, 1, this->acc_dev_addr));
      to_write = SET_FIELD(to_read, ACC_RANGE, ACC_RANGE_12G);
      assert(write_reg( ACC_RANGE_ADDR, &to_write, 1, this->acc_dev_addr));
      break;
    case BMI088_Sens::Range_acc::Mg24:
      to_write = ACC_RANGE_24G;
      this->range_integer = 24;
      assert(read_reg( ACC_RANGE_ADDR, &to_read, 1, this->acc_dev_addr));
      to_write = SET_FIELD(to_read, ACC_RANGE, ACC_RANGE_24G);
      assert(write_reg( ACC_RANGE_ADDR, &to_write, 1, this->acc_dev_addr));
      break;
  }
  return true;
}

bool BMI088_Sens::set_odr_acc(const BMI088_Sens::Odr& odr){
  uint8_t to_write = 0;
  uint8_t to_read = 0;
  uint8_t value = 0;
  switch(odr){
    case BMI088_Sens::Odr::ODR_1600HZ_BW_280HZ:
      value = (0x0A << 4) | 0x0C;
      break;
    case BMI088_Sens::Odr::ODR_1600HZ_BW_234HZ:{
      value = (0x09 << 4) | 0x0C;
      break;
    }
    case BMI088_Sens::Odr::ODR_1600HZ_BW_145HZ:{
      value = (0x08 << 4) | 0x0C;
      break;
    }
    case BMI088_Sens::Odr::ODR_800HZ_BW_230HZ:{
      value = (0x0A << 4) | 0x0B;
      break;
    }
    case BMI088_Sens::Odr::ODR_800HZ_BW_140HZ:{
      value = (0x09 << 4) | 0x0B;
      break;
    }
    case BMI088_Sens::Odr::ODR_800HZ_BW_80HZ:{
      value = (0x08 << 4) | 0x0B;
      break;
    }
    case BMI088_Sens::Odr::ODR_400HZ_BW_75HZ:{
      value = (0x09 << 4) | 0x0A;
      break;
    }
    case BMI088_Sens::Odr::ODR_400HZ_BW_40HZ:{
      value = (0x08 << 4) | 0x0A;
      break;
    }
    case BMI088_Sens::Odr::ODR_200HZ_BW_80HZ:{
      value = (0x0A << 4) | 0x09;
      break;
    }
    case BMI088_Sens::Odr::ODR_200HZ_BW_38HZ:{
      value = (0x09 << 4) | 0x09;
      break;
    }
    case BMI088_Sens::Odr::ODR_200HZ_BW_20HZ:{
      value = (0x08 << 4) | 0x09;
      break;
    }
    case BMI088_Sens::Odr::ODR_100HZ_BW_40HZ:{
      value = (0x0A << 4) | 0x08;
      break;
    }
    case BMI088_Sens::Odr::ODR_100HZ_BW_19HZ:{
      value = (0x09 << 4) | 0x08;
      break;
    }
    case BMI088_Sens::Odr::ODR_100HZ_BW_10HZ:{
      value = (0x08 << 4) | 0x08;
      break;
    }
    case BMI088_Sens::Odr::ODR_50HZ_BW_20HZ:{
      value = (0x0A << 4) | 0x07;
      break;
    }
    case BMI088_Sens::Odr::ODR_50HZ_BW_9HZ:{
      value = (0x09 << 4) | 0x07;
      break;
    }
    case BMI088_Sens::Odr::ODR_50HZ_BW_5HZ:{
      value = (0x08 << 4) | 0x07;
      break;
    }
    case BMI088_Sens::Odr::ODR_25HZ_BW_10HZ:{
      value = (0x0A << 4) | 0x06;
      break;
    }
    case BMI088_Sens::Odr::ODR_25HZ_BW_5HZ:{
      value = (0x09 << 4) | 0x06;
      break;
    }
    case BMI088_Sens::Odr::ODR_25HZ_BW_3HZ:{
      value = (0x08 << 4) | 0x06;
      break;
    }
    case BMI088_Sens::Odr::ODR_12_5HZ_BW_5HZ:{
      value = (0x0A << 4) | 0x05;
      break;
    }
    case BMI088_Sens::Odr::ODR_12_5HZ_BW_2HZ:{
      value = (0x09 << 4) | 0x05;
      break;
    }
    case BMI088_Sens::Odr::ODR_12_5HZ_BW_1HZ:{
      value = (0x08 << 4) | 0x05;
      break;
    }
    default:{
      value = (0x0A << 4) | 0x0C;
      break;
    }
  }
  to_write = SET_FIELD(to_write, ACC_ODR, value);
  assert(write_reg( ACC_ODR_ADDR, &to_write, 1, this->acc_dev_addr));
  osDelay(250);
  assert(read_reg( ACC_ODR_ADDR, &to_read, 1, this->acc_dev_addr));
  return to_read == to_write;
}

bool BMI088_Sens::set_power_acc(const bool& power){
	uint8_t to_write = 0;
	uint8_t value = (power) ? ACC_ENABLE_CMD : ACC_DISABLE_CMD;
	to_write = SET_FIELD(to_write,ACC_POWER_CNTRL, value);

	uint8_t res_power_cntrl = write_reg( ACC_POWER_CNTRL_ADDR, &to_write, 1, this->acc_dev_addr);
	osDelay(500);
	uint8_t to_read = 0;
	uint8_t res_read_power_cntrl = read_reg( ACC_POWER_CNTRL_ADDR, &to_read, 1, this->acc_dev_addr);
	return to_read==to_write;
}

bool BMI088_Sens::set_mode_acc(const bool& mode){
	uint8_t reg_write = 0;
	uint8_t val_write = (mode) ? ACC_ACTIVE_MODE_CMD : ACC_SUSPEND_MODE_CMD;
	reg_write = SET_FIELD(reg_write, ACC_POWER_CONF, val_write);
	assert(write_reg(ACC_POWER_CONF_ADDR, &val_write, 1, this->acc_dev_addr) == true);
	osDelay(100);
	uint8_t val_read = 0;
	assert(read_reg(ACC_POWER_CONF_ADDR, &val_read, 1, this->acc_dev_addr) == true);
	return val_read == val_write;
	//	uint8_t reg_write = 0;
	//	uint8_t val_write = (mode) ? ACC_ACTIVE_MODE_CMD : ACC_SUSPEND_MODE_CMD;
	//	reg_write = SET_FIELD(reg_write, ACC_POWER_CONF, val_write);
	//	uint8_t write_acc_conf_res = write_reg(sens->i2c, sens->acc_dev_addr, ACC_POWER_CONF_ADDR, &val_write, 1);
	//	osDelay(5);
	//	uint8_t val_read = 0;
	//	uint8_t read_acc_conf_res = read_reg(sens->i2c, sens->acc_dev_addr, ACC_POWER_CONF_ADDR, &val_read, 1);
	//	return val_read == val_write;
}

bool BMI088_Sens::self_test_acc(){
  uint8_t write_register = 0;
  uint8_t read_register = 0;
  float accel_pos_mg[3];
  float accel_neg_mg[3];
  if(this->set_range_acc(BMI088_Sens::Range_acc::Mg24) == false){
	  return false;
  }
  if(this->set_odr_acc(BMI088_Sens::Odr::ODR_1600HZ_BW_145HZ) == false){
	  return false;
  }
  osDelay(100);
  write_register = SET_FIELD(write_register, ACC_SELF_TEST, ACC_POS_SELF_TEST);
  assert(write_reg(ACC_SELF_TEST_ADDR, &write_register, 1, this->acc_dev_addr) == true);
  osDelay(100);
  this->get_accel();
  for(uint8_t i = 0; i<3; i++){
    accel_pos_mg[i] = this->accel_mss[i]/G * 1000.0f;
  }
  write_register = 0;
  write_register = SET_FIELD(write_register, ACC_SELF_TEST, ACC_NEG_SELF_TEST);
  assert(write_reg(ACC_SELF_TEST_ADDR, &write_register, 1, this->acc_dev_addr) == true);

  osDelay(100);
  this->get_accel();
  for(uint8_t i = 0; i<3; i++){
    accel_neg_mg[i] = this->accel_mss[i]/G * 1000.0f;
  }

  write_register = SET_FIELD(write_register, ACC_SELF_TEST, ACC_DIS_SELF_TEST);
  assert(write_reg(ACC_SELF_TEST_ADDR, &write_register, 1, this->acc_dev_addr) == true);

	if((fabs(accel_pos_mg[0]-accel_neg_mg[0]) >= 1000) && (fabs(accel_pos_mg[1]-accel_neg_mg[1]) >=1000) && (fabs(accel_pos_mg[2]-accel_neg_mg[2])>=500)){
		return true;
	}
	else{
		return false;
	}
}

void BMI088_Sens::get_accel(){
	uint16_t unsigned_accel;
	uint8_t buff[9];
	int16_t temp_int_accel[3];
	uint8_t res_read_accel = read_reg(ACC_ACCEL_DATA_ADDR, buff, 9, this->acc_dev_addr);
	temp_int_accel[0] = (buff[1] << 8 ) | buff[0];
	temp_int_accel[1] = (buff[3] << 8 ) | buff[2];
	temp_int_accel[2] = (buff[5] << 8 ) | buff[4];

	//this->accel_mss[0] = (float)temp_int_accel[0]*range_integer;
	//this->accel_mss[0];
	//this->accel_mss[0] = ((float)((float)temp_int_accel[0]*this->tr_x[0] + (float)temp_int_accel[1]*this->tr_x[1] + (float)temp_int_accel[2]*this->tr_x[2])) / 32768.0f * (float)this->range_integer;

	this->accel_mss[0] = (float)(temp_int_accel[0]*this->tr_x[0] + temp_int_accel[1]*this->tr_x[1] + temp_int_accel[2]*this->tr_x[2]) / 32767.0f * this->range_integer*G;
	this->accel_mss[1] = (float)(temp_int_accel[0]*this->tr_y[0] + temp_int_accel[1]*this->tr_y[1] + temp_int_accel[2]*this->tr_y[2]) / 32767.0f * this->range_integer*G;
	this->accel_mss[2] = (float)(temp_int_accel[0]*this->tr_z[0] + temp_int_accel[1]*this->tr_z[1] + temp_int_accel[2]*this->tr_z[2]) / 32767.0f * this->range_integer*G;

  return;
}

bool BMI088_Sens::soft_reset_gyro(){
  uint8_t reg = 0; 
  reg = SET_FIELD(reg, GYRO_SOFT_RESET, GYRO_RESET_CMD);
  uint8_t res_soft_reset = this->write_reg(GYRO_SOFT_RESET_ADDR, &reg, 1, this->gyro_dev_addr);
  osDelay(100);
  if(res_soft_reset != HAL_OK){
    return false;
  }
  else{
    return true;
  }
}

bool BMI088_Sens::set_range_gyro(const BMI088_Sens::Range_gyro& range_gyro){
  switch(range_gyro){
    case BMI088_Sens::Range_gyro::Dps_2000:
    {
		  uint8_t to_write = 0; uint8_t to_read = 0;
		  to_write = SET_FIELD(to_write, GYRO_RANGE, GYRO_RANGE_2000_DPS);
		  assert(this->write_reg(GYRO_RANGE_ADDR, &to_write, 1, this->gyro_dev_addr) == true);
		  osDelay(50);
		  assert(this->read_reg(GYRO_RANGE_ADDR, &to_read, 1, this->gyro_dev_addr) == true);
		  this->gyro_range_integer = 2000;
		  return to_read == to_write;
    }
    case BMI088_Sens::Range_gyro::Dps_1000:
    {
		  uint8_t to_write = 0; uint8_t to_read = 0;
		  to_write = SET_FIELD(to_write, GYRO_RANGE, GYRO_RANGE_1000_DPS);
		  assert(this->write_reg(GYRO_RANGE_ADDR, &to_write, 1, this->gyro_dev_addr) == true);
		  osDelay(50);
		  assert(this->read_reg(GYRO_RANGE_ADDR, &to_read, 1, this->gyro_dev_addr) == true);
		  this->gyro_range_integer = 1000;
		  return to_read == to_write;
    }
    case BMI088_Sens::Range_gyro::Dps_500:
    {
		  uint8_t to_write = 0; uint8_t to_read = 0;
		  to_write = SET_FIELD(to_write, GYRO_RANGE, GYRO_RANGE_500_DPS);
		  assert(this->write_reg(GYRO_RANGE_ADDR, &to_write, 1, this->gyro_dev_addr) == true);
		  osDelay(50);
		  assert(this->read_reg(GYRO_RANGE_ADDR, &to_read, 1, this->gyro_dev_addr) == true);
		  this->gyro_range_integer = 500;
		  return to_read == to_write;
    }
    case BMI088_Sens::Range_gyro::Dps_250:
    {
      uint8_t to_write = 0; uint8_t to_read = 0;
	  to_write = SET_FIELD(to_write, GYRO_RANGE, GYRO_RANGE_250_DPS);
      assert(this->write_reg(GYRO_RANGE_ADDR, &to_write, 1, this->gyro_dev_addr) == true);
      osDelay(50);
      assert(this->read_reg(GYRO_RANGE_ADDR, &to_read, 1, this->gyro_dev_addr) == true);
      this->gyro_range_integer = 250;
      return to_read == to_write;
    }
    case BMI088_Sens::Range_gyro::Dps_125:
    {
      uint8_t to_write = 0; uint8_t to_read = 0;
      to_write = SET_FIELD(to_write, GYRO_RANGE, GYRO_RANGE_125_DPS);
      assert(this->write_reg(GYRO_RANGE_ADDR, &to_write, 1, this->gyro_dev_addr) == true);
      osDelay(50);
      assert(this->read_reg(GYRO_RANGE_ADDR, &to_read, 1, this->gyro_dev_addr) == true);
      this->gyro_range_integer = 125;
      return to_read == to_write;
    }
  }
  return false;
}

bool BMI088_Sens::set_drdy_int_gyro(bool enable){
  uint8_t to_write = 0;
  uint8_t to_read = 0;
  uint8_t value = (enable) ? GYRO_ENABLE_DRDY_INT : GYRO_DIS_DRDY_INT;
  to_write = SET_FIELD(to_write, GYRO_INT_CNTRL, value);
  assert(this->write_reg(GYRO_INT_CNTRL_ADDR, &to_write, 1, this->gyro_dev_addr) == true);
  osDelay(50);
  assert(this->read_reg(GYRO_INT_CNTRL_ADDR, &to_read, 1, this->gyro_dev_addr) == true);
  return to_read == to_write;
}

bool BMI088_Sens::set_odr_gyro(const BMI088_Sens::Odr_gyro& odr_gyro){
  uint8_t to_write = 0;
  uint8_t to_read = 0;

  switch(odr_gyro){
    case BMI088_Sens::Odr_gyro::ODR_2000HZ_BW_532:
      to_write = SET_FIELD(to_write, GYRO_ODR, GYRO_ODR_2000HZ_BW_532);
      assert(write_reg(GYRO_ODR_ADDR, &to_write, 1, this->gyro_dev_addr) == true);
      osDelay(50);
      assert(read_reg(GYRO_ODR_ADDR, &to_read, 1, this->gyro_dev_addr) == true);
      return to_read == to_write;
    case BMI088_Sens::Odr_gyro::ODR_2000HZ_BW_230:
      to_write = SET_FIELD(to_write, GYRO_ODR, GYRO_ODR_2000HZ_BW_230);
      assert(write_reg(GYRO_ODR_ADDR, &to_write, 1, this->gyro_dev_addr) == true);
      osDelay(50);
      assert(read_reg(GYRO_ODR_ADDR, &to_read, 1, this->gyro_dev_addr) == true);
      return to_read == to_write;
    case BMI088_Sens::Odr_gyro::ODR_1000HZ_BW_116:
      to_write = SET_FIELD(to_write, GYRO_ODR, GYRO_ODR_1000HZ_BW_116);
      assert(write_reg(GYRO_ODR_ADDR, &to_write, 1, this->gyro_dev_addr) == true);
      osDelay(50);
      assert(read_reg(GYRO_ODR_ADDR, &to_read, 1, this->gyro_dev_addr) == true);
      return to_read == to_write;
    case BMI088_Sens::Odr_gyro::ODR_400HZ_BW_47:
      to_write = SET_FIELD(to_write, GYRO_ODR, GYRO_ODR_400HZ_BW_47);
      assert(write_reg(GYRO_ODR_ADDR, &to_write, 1, this->gyro_dev_addr) == true);
      osDelay(50);
      assert(read_reg(GYRO_ODR_ADDR, &to_read, 1, this->gyro_dev_addr) == true);
      return to_read == to_write;
    case BMI088_Sens::Odr_gyro::ODR_200HZ_BW_64:
      to_write = SET_FIELD(to_write, GYRO_ODR, GYRO_ODR_200HZ_BW_64);
      assert(write_reg(GYRO_ODR_ADDR, &to_write, 1, this->gyro_dev_addr) == true);
      osDelay(50);
      assert(read_reg(GYRO_ODR_ADDR, &to_read, 1, this->gyro_dev_addr) == true);
      return to_read == to_write;
    case BMI088_Sens::Odr_gyro::ODR_200HZ_BW_23:
      to_write = SET_FIELD(to_write, GYRO_ODR, GYRO_ODR_200HZ_BW_23);
      assert(write_reg(GYRO_ODR_ADDR, &to_write, 1, this->gyro_dev_addr) == true);
      osDelay(50);
      assert(read_reg(GYRO_ODR_ADDR, &to_read, 1, this->gyro_dev_addr) == true);
      return to_read == to_write;
    case BMI088_Sens::Odr_gyro::ODR_100HZ_BW_32:
      to_write = SET_FIELD(to_write, GYRO_ODR, GYRO_ODR_100HZ_BW_32);
      assert(write_reg(GYRO_ODR_ADDR, &to_write, 1, this->gyro_dev_addr) == true);
      osDelay(50);
      assert(read_reg(GYRO_ODR_ADDR, &to_read, 1, this->gyro_dev_addr) == true);
      return to_read == to_write;
    case BMI088_Sens::Odr_gyro::ODR_100HZ_BW_12:
      to_write = SET_FIELD(to_write, GYRO_ODR, GYRO_ODR_100HZ_BW_12);
      assert(write_reg(GYRO_ODR_ADDR, &to_write, 1, this->gyro_dev_addr) == true);
      osDelay(50);
      assert(read_reg(GYRO_ODR_ADDR, &to_read, 1, this->gyro_dev_addr) == true);
      return to_read == to_write;
  }
  return false;
}

void BMI088_Sens::get_angle_accel(){
	int16_t gyro[3];
	uint8_t buff[6];
	assert(read_reg(GYRO_DATA_ADDR, buff, 6, this->gyro_dev_addr) == true);
	gyro[0] = (buff[1] << 8) | buff[0];
	gyro[1] = (buff[3] << 8) | buff[2];
	gyro[2] = (buff[5] << 8) | buff[4];

	this->angle_accel_radss[0] = (float)(gyro[0]*this->tr_x[0] + gyro[1]*this->tr_x[1] + gyro[2]*this->tr_x[2])/32767.0f * this->gyro_range_integer*this->D2R;
	this->angle_accel_radss[1] = (float)(gyro[0]*this->tr_y[0] + gyro[1]*this->tr_y[1] + gyro[2]*this->tr_y[2])/32767.0f * this->gyro_range_integer*this->D2R;
	this->angle_accel_radss[2] = (float)(gyro[0]*this->tr_z[0] + gyro[1]*this->tr_z[1] + gyro[2]*this->tr_z[2])/32767.0f * this->gyro_range_integer*this->D2R;

}

void BMI088_Sens::get_angle_accel_wdata(float* acc){
	this->get_angle_accel();
	acc[0] = this->angle_accel_radss[0];
	acc[1] = this->angle_accel_radss[1];
	acc[2] = this->angle_accel_radss[2];

}

void BMI088_Sens::get_accel_wdata(float* acc){
	this->get_accel();
	acc[0] = this->accel_mss[0];
	acc[1] = this->accel_mss[1];
	acc[2] = this->accel_mss[2];

	return;
}

BMI088_Sens::xyz BMI088_Sens::get_last_accel(){
	BMI088_Sens::xyz to_ret;
	this->get_accel();
	to_ret.x = this->accel_mss[0];
	to_ret.y = this->accel_mss[1];
	to_ret.z = this->accel_mss[2];
	return to_ret;
}

BMI088_Sens::xyz BMI088_Sens::get_last_accel_w_bias(){
	this->get_accel();
	return BMI088_Sens::xyz{
		this->accel_mss[0]-this->bias_offset_acc.x,
		this->accel_mss[1]-this->bias_offset_acc.y,
		this->accel_mss[2]-this->bias_offset_acc.z
	};
}

BMI088_Sens::xyz BMI088_Sens::get_last_angle_accel(){
	BMI088_Sens::xyz to_ret;
	this->get_angle_accel();
	to_ret.x = this->angle_accel_radss[0];
	to_ret.y = this->angle_accel_radss[1];
	to_ret.z = this->angle_accel_radss[2];
	return to_ret;
}

BMI088_Sens::xyz BMI088_Sens::get_last_angle_accel_w_bias(){
	this->get_angle_accel();
	return BMI088_Sens::xyz{
		this->angle_accel_radss[0]-this->bias_offset_gyro.x,
		this->angle_accel_radss[1]-this->bias_offset_gyro.y,
		this->angle_accel_radss[2]-this->bias_offset_gyro.z
	};
}


void BMI088_Sens::calibrate_acc(int sec){
	float sumx = 0.0; float sumy = 0.0; float sumz = 0.0;
	uint8_t counter = 0;
    auto start = std::chrono::steady_clock::now();
    BMI088_Sens::xyz data;
	while(true){
		data = this->get_last_accel();
		sumx += data.x; sumy+=data.y; sumz+=data.z;
		counter ++;
		if(std::chrono::steady_clock::now()-start > std::chrono::seconds(sec)){
			break;
		}
	}
	this->bias_offset_acc = BMI088_Sens::xyz{sumx/((float)counter), sumy/((float)counter), sumz/((float)counter)};
}

void BMI088_Sens::calibrate_gyro(int sec){
	float sumx = 0.0; float sumy = 0.0; float sumz = 0.0;
		uint8_t counter = 0;
	    auto start = std::chrono::steady_clock::now();
	    BMI088_Sens::xyz data;
		while(true){
			data = this->get_last_angle_accel();
			sumx += data.x; sumy+=data.y; sumz+=data.z;
			counter ++;
			if(std::chrono::steady_clock::now()-start > std::chrono::seconds(sec)){
				break;
			}
		}
		this->bias_offset_gyro = BMI088_Sens::xyz{sumx/((float)counter), sumy/((float)counter), sumz/((float)counter)};
}
//bool soft_reset_acc(struct BMI088_Sens* sens){
//	uint8_t reg = 0;
//	reg = SET_FIELD(reg,ACC_SOFT_RESET, ACC_RESET_CMD);
//	uint8_t res_soft_reset = write_reg(sens->i2c, sens->acc_dev_addr, BMI08_REG_ACCEL_SOFTRESET, &reg, 1);
//	if(res_soft_reset != HAL_OK){
//		return false;
//	}
//	else{
//		return true;
//	}
//}
//
//bool soft_reset_gyro(struct BMI088_Sens* sens){
//	uint8_t reg = 0;
//	reg = SET_FIELD(reg, GYRO_SOFT_RESET, GYRO_RESET_CMD);
//	uint8_t res_soft_reset = write_reg(sens->i2c, sens->gyro_dev_addr, GYRO_SOFT_RESET_ADDR, &reg, 1);
//	osDelay(50);
//	if(res_soft_reset != HAL_OK){
//		return false;
//	}
//	else{
//		return true;
//	}
//}
//
//bool set_power_acc(struct BMI088_Sens* sens, bool power){
//	uint8_t to_write = 0;
//	uint8_t value = (power) ? ACC_ENABLE_CMD : ACC_DISABLE_CMD;
//	to_write = SET_FIELD(to_write,ACC_POWER_CNTRL, value);
//
//	uint8_t res_power_cntrl = write_reg(sens->i2c, sens->acc_dev_addr, ACC_POWER_CNTRL_ADDR, &to_write, 1);
//	osDelay(500);
//	uint8_t to_read = 0;
//	uint8_t res_read_power_cntrl = read_reg(sens->i2c, sens->acc_dev_addr, ACC_POWER_CNTRL_ADDR, &to_read, 1);
//	return to_read==to_write;
//}
//
//bool set_mode_acc(struct BMI088_Sens* sens, bool mode){
//	uint8_t reg_write = 0;
//	uint8_t val_write = (mode) ? ACC_ACTIVE_MODE_CMD : ACC_SUSPEND_MODE_CMD;
//	reg_write = SET_FIELD(reg_write, ACC_POWER_CONF, val_write);
//	uint8_t write_acc_conf_res = write_reg(sens->i2c, sens->acc_dev_addr, ACC_POWER_CONF_ADDR, &val_write, 1);
//	osDelay(5);
//	uint8_t val_read = 0;
//	uint8_t read_acc_conf_res = read_reg(sens->i2c, sens->acc_dev_addr, ACC_POWER_CONF_ADDR, &val_read, 1);
//	return val_read == val_write;
//}
//
//bool set_range_acc(struct BMI088_Sens* sens, uint8_t range){
//	uint8_t to_read = 0;
//	uint8_t to_write = 0;
//	uint8_t read_range_res = read_reg(sens->i2c, sens->acc_dev_addr, ACC_RANGE_ADDR, &to_read, 1);
//	to_write = SET_FIELD(to_read, ACC_RANGE, range);
//	uint8_t write_range_res = write_reg(sens->i2c, sens->acc_dev_addr, ACC_RANGE_ADDR, &to_write, 1);
//	read_range_res = read_reg(sens->i2c, sens->acc_dev_addr,ACC_RANGE_ADDR, &to_read, 1);
//	if(range == BMI088_ACCEL_RANGE_24G){
//		sens->acc_range_real = 24*G;
//	}
//	else if(range == BMI088_ACCEL_RANGE_12G){
//		sens->acc_range_real = 12*G;
//	}
//	else if(range == BMI088_ACCEL_RANGE_6G){
//		sens->acc_range_real = 6*G;
//	}
//	else if(range == BMI088_ACCEL_RANGE_3G){
//		sens->acc_range_real = 3*G;
//	}
//	return to_read == to_write;
//}
//
//bool set_range_gyro(struct BMI088_Sens* sens, uint8_t range){
//	uint8_t to_read = 0;
//	uint8_t to_write = 0;
//	to_write = SET_FIELD(to_write, GYRO_RANGE, range);
//	uint8_t write_range_res = write_reg(sens->i2c, sens->gyro_dev_addr, GYRO_RANGE_ADDR, &to_write, 1);
//	osDelay(1);
//	uint8_t read_range_res = read_reg(sens->i2c, sens->gyro_dev_addr, GYRO_RANGE_ADDR, &to_read, 1);
//	if(to_read != to_write){
//		return false;
//	}
//	if(range == GYRO_RANGE_2000_DPS){
//		sens->gyro_range_real = 2000;
//	}
//	else if(range == GYRO_RANGE_1000_DPS){
//		sens->gyro_range_real = 1000;
//	}
//	else if(range == GYRO_RANGE_500_DPS){
//		sens->gyro_range_real = 500;
//	}
//	else if(range == GYRO_RANGE_250_DPS){
//		sens->gyro_range_real = 250;
//	}
//	else if(range == GYRO_RANGE_125_DPS){
//		sens->gyro_range_real = 125;
//	}
//	return true;
//}
//
//bool self_test_acc(struct BMI088_Sens* sens){
//	uint8_t write_register = 0;
//	float accel_pos_mg[3];
//	float accel_neg_mg[3];
//	set_range_acc(sens, BMI088_ACCEL_RANGE_24G);
//	set_odr_acc(sens, ODR_1600HZ_BW_145HZ);
//	osDelay(3);
//	write_register = SET_FIELD(write_register, ACC_SELF_TEST, ACC_POS_SELF_TEST);
//	uint8_t write_reg_self_test = write_reg(sens->i2c, sens->acc_dev_addr, ACC_SELF_TEST_ADDR, &write_register, 1);
//	osDelay(51);
//	get_accel(sens, true);
//	for(uint8_t i = 0; i<3; i++){
//		accel_pos_mg[i] = sens->accel_mss[i]/G * 1000.0f;
//	}
//
//	write_register = SET_FIELD(write_register, ACC_SELF_TEST, ACC_NEG_SELF_TEST);
//	write_reg_self_test = write_reg(sens->i2c, sens->acc_dev_addr, ACC_SELF_TEST_ADDR, &write_register, 1);
//
//	osDelay(51);
//	get_accel(sens, true);
//	for(uint8_t i = 0; i<3; i++){
//		accel_neg_mg[i] = sens->accel_mss[i]/G * 1000.0f;
//	}
//
//	write_register = SET_FIELD(write_register, ACC_SELF_TEST, ACC_DIS_SELF_TEST);
//	write_reg_self_test = write_reg(sens->i2c, sens->acc_dev_addr, ACC_SELF_TEST_ADDR, &write_register, 1);
//
//	//read the sensor
//	if((fabs(accel_pos_mg[0]-accel_neg_mg[0]) >= 1000) && (fabs(accel_pos_mg[1]-accel_neg_mg[1]) >=1000) && (fabs(accel_pos_mg[2]-accel_neg_mg[2])>=500)){
//		return true;
//	}
//	else{
//		return false;
//	}
//
//
//}
//
//void initialize_accel(struct BMI088_Sens* sens){
//
//	sens->acc_dev_addr = BMI08_ACCEL_I2C_ADDR_PRIMARY;
//	set_transformations_acc(sens);
//
//	if(get_acc_chip_id(sens) != BMI088_ACCEL_CHIP_ID){
//		return;
//	}
//	soft_reset_acc(sens);
//
//	if(!set_power_acc(sens, true)){
//		return;
//	}
//	if(!set_mode_acc(sens, true)){
//		return;
//	}
//	if(!set_range_acc(sens, BMI088_ACCEL_RANGE_3G)){
//		return;
//	}
//
//
//	if(!self_test_acc(sens)){
//		return;
//	}
//
//	set_range_acc(sens, BMI088_ACCEL_RANGE_3G);
//
//	set_odr_acc(sens, ODR_100HZ_BW_10HZ);
//
//	soft_reset_acc(sens);
//
//	if(!set_power_acc(sens, true)){
//		return;
//	}
//}
//
//int initialize_gyro(struct BMI088_Sens* sens){
//	sens->gyro_dev_addr = BMI08_GYRO_I2C_ADDR_PRIMARY;
//
//	if(get_gyro_chip_id(sens) != GYRO_CHIP_ID){
//		return -1;
//	}
//	soft_reset_gyro(sens);
//	osDelay(50);
//
//	if(!set_range_gyro(sens, GYRO_RANGE_1000_DPS)){
//		return -2;
//	}
//
//	if(!set_drdy_int_gyro(sens, true)){
//		return -3;
//	}
//	if(!set_odr_gyro(sens, GYRO_ODR_100HZ_BW_12)){
//		return -4;
//	}
//
//	return 0;
//}
//
//uint8_t get_gyro_chip_id(struct BMI088_Sens* sens){
//	uint8_t chip_id = 0;
//	read_reg(sens->i2c, sens->gyro_dev_addr, GYRO_CHIP_ID_ADDR, &chip_id, 1);
//	return chip_id;
//}
//
//uint8_t get_acc_chip_id(struct BMI088_Sens* sens){
//  uint8_t chip_id = 0;
//  read_reg(sens->i2c, sens->acc_dev_addr, BMI08_REG_ACCEL_CHIP_ID, &chip_id, 1);
//  return chip_id;
//}
//
//bool set_drdy_int_gyro(struct BMI088_Sens* sens, bool enable){
//	uint8_t to_write = 0;
//	uint8_t to_read = 0;
//	uint8_t value = (enable) ? GYRO_ENABLE_DRDY_INT : GYRO_DIS_DRDY_INT;
//	to_write = SET_FIELD(to_write, GYRO_INT_CNTRL, value);
//	uint8_t write_enable_res = write_reg(sens->i2c, sens->gyro_dev_addr, GYRO_INT_CNTRL_ADDR, &to_write, 1);
//	osDelay(1);
//	uint8_t read_enable_res = read_reg(sens->i2c, sens->gyro_dev_addr, GYRO_INT_CNTRL_ADDR, &to_read, 1);
//	return (to_write == to_read);
//}
//
//void set_odr_acc(struct BMI088_Sens* sens, enum Odr odr){
//	uint8_t to_write = 0;
//	uint8_t to_read = 0;
//	uint8_t value = 0;
//	switch(odr){
//	case ODR_1600HZ_BW_280HZ:{
//		value = (0x0A << 4) | 0x0C;
//		break;
//	}
//	case ODR_1600HZ_BW_234HZ:{
//		value = (0x09 << 4) | 0x0C;
//		break;
//	}
//	case ODR_1600HZ_BW_145HZ:{
//		value = (0x08 << 4) | 0x0C;
//		break;
//	}
//	case ODR_800HZ_BW_230HZ:{
//		value = (0x0A << 4) | 0x0B;
//		break;
//	}
//	case ODR_800HZ_BW_140HZ:{
//		value = (0x09 << 4) | 0x0B;
//		break;
//	}
//	case ODR_800HZ_BW_80HZ:{
//		value = (0x08 << 4) | 0x0B;
//		break;
//	}
//	case ODR_400HZ_BW_145HZ:{
//		value = (0x0A << 4) | 0x0A;
//		break;
//	}
//	case ODR_400HZ_BW_75HZ:{
//		value = (0x09 << 4) | 0x0A;
//		break;
//	}
//	case ODR_400HZ_BW_40HZ:{
//		value = (0x08 << 4) | 0x0A;
//		break;
//	}
//	case ODR_200HZ_BW_80HZ:{
//		value = (0x0A << 4) | 0x09;
//		break;
//	}
//	case ODR_200HZ_BW_38HZ:{
//		value = (0x09 << 4) | 0x09;
//		break;
//	}
//	case ODR_200HZ_BW_20HZ:{
//		value = (0x08 << 4) | 0x09;
//		break;
//	}
//	case ODR_100HZ_BW_40HZ:{
//		value = (0x0A << 4) | 0x08;
//		break;
//	}
//	case ODR_100HZ_BW_19HZ:{
//		value = (0x09 << 4) | 0x08;
//		break;
//	}
//	case ODR_100HZ_BW_10HZ:{
//		value = (0x08 << 4) | 0x08;
//		break;
//	}
//	case ODR_50HZ_BW_20HZ:{
//		value = (0x0A << 4) | 0x07;
//		break;
//	}
//	case ODR_50HZ_BW_9HZ:{
//		value = (0x09 << 4) | 0x07;
//		break;
//	}
//	case ODR_50HZ_BW_5HZ:{
//		value = (0x08 << 4) | 0x07;
//		break;
//	}
//	case ODR_25HZ_BW_10HZ:{
//		value = (0x0A << 4) | 0x06;
//		break;
//	}
//	case ODR_25HZ_BW_5HZ:{
//		value = (0x09 << 4) | 0x06;
//		break;
//	}
//	case ODR_25HZ_BW_3HZ:{
//		value = (0x08 << 4) | 0x06;
//		break;
//	}
//	case ODR_12_5HZ_BW_5HZ:{
//		value = (0x0A << 4) | 0x05;
//		break;
//	}
//	case ODR_12_5HZ_BW_2HZ:{
//		value = (0x09 << 4) | 0x05;
//		break;
//	}
//	case ODR_12_5HZ_BW_1HZ:{
//		value = (0x08 << 4) | 0x05;
//		break;
//	}
//	default:{
//		value = (0x0A << 4) | 0x0C;
//		break;
//	}
//	}
//
//
//	to_write = SET_FIELD(to_write, ACC_ODR, value);
//	uint8_t write_odr_res = write_reg(sens->i2c, sens->acc_dev_addr, ACC_ODR_ADDR, &to_write, 1);
//	osDelay(1);
//	uint8_t read_odr_res = read_reg(sens->i2c, sens->acc_dev_addr, ACC_ODR_ADDR, &to_read, 1);
//	return ;
//}
//
//bool set_odr_gyro(struct BMI088_Sens* sens, uint8_t odr){
//	uint8_t to_write = 0;
//	uint8_t to_read = 0;
//	to_write = SET_FIELD(to_write, GYRO_ODR, odr);
//	uint8_t write_odr_res = write_reg(sens->i2c, sens->gyro_dev_addr, GYRO_ODR_ADDR, &to_write, 1);
//	osDelay(1);
//	uint8_t read_odr_res = read_reg(sens->i2c, sens->gyro_dev_addr, GYRO_ODR_ADDR, &to_read, 1);
//
//	return (to_read == to_write);
//}
//
//void get_accel_wdata(struct BMI088_Sens* sens, float* data){
//	uint16_t unsigned_accel;
//	int16_t temp_int_accel[3];
//	uint8_t buffer[9];
//	read_reg(sens->i2c, sens->acc_dev_addr, ACC_ACCEL_DATA_ADDR, buffer, 9);
//	temp_int_accel[0] = (buffer[1]<<8) | buffer[0];
//	temp_int_accel[1] = (buffer[3]<<8) | buffer[2];
//	temp_int_accel[2] = (buffer[5]<<8) | buffer[4];
//
//	data[0] = ((float)(temp_int_accel[0]*sens->tr_x[0] + temp_int_accel[1]*sens->tr_x[1] + temp_int_accel[2]*sens->tr_x[2])) / 32768.0f * 3;
//	data[1] = ((float)(temp_int_accel[0]*sens->tr_y[0] + temp_int_accel[1]*sens->tr_y[1] + temp_int_accel[2]*sens->tr_y[2])) / 32768.0f * 3;
//	data[2] = ((float)(temp_int_accel[0]*sens->tr_z[0] + temp_int_accel[1]*sens->tr_z[1] + temp_int_accel[2]*sens->tr_z[2])) / 32768.0f * 3;
//}
//
//
//void get_accel(struct BMI088_Sens* sens, bool mg){
//	uint16_t unsigned_accel;
//	uint8_t buff[9];
//	int16_t temp_int_accel[3];
//	uint8_t res_read_accel = read_reg(sens->i2c, sens->acc_dev_addr, ACC_ACCEL_DATA_ADDR, buff, 9);
//	temp_int_accel[0] = (buff[1] << 8 ) | buff[0];
//	temp_int_accel[1] = (buff[3] << 8 ) | buff[2];
//	temp_int_accel[2] = (buff[5] << 8 ) | buff[4];
//	sens->accel_mss[0] = ((float)(temp_int_accel[0]*sens->tr_x[0] + temp_int_accel[1]*sens->tr_x[1] + temp_int_accel[2]*sens->tr_x[2])) / 32768.0f * sens->acc_range_real;
//	sens->accel_mss[1] = ((float)(temp_int_accel[0]*sens->tr_y[0] + temp_int_accel[1]*sens->tr_y[1] + temp_int_accel[2]*sens->tr_y[2])) / 32768.0f * sens->acc_range_real;
//	sens->accel_mss[2] = ((float)(temp_int_accel[0]*sens->tr_z[0] + temp_int_accel[1]*sens->tr_z[1] + temp_int_accel[2]*sens->tr_z[2])) / 32768.0f * sens->acc_range_real;
//	//sens->current_time_counter = (sens->buffer[8] << 16) | (sens->buffer[7] << 8) | buffer[6];
//}
//
//void get_temperature(struct BMI088_Sens* sens){
//	uint8_t read_data_res = read_reg(sens->i2c, sens->acc_dev_addr, ACC_TEMP_DATA_ADDR, sens->buffer, 2);
//	uint16_t temp_uint = sens->buffer[0]*8 + (sens->buffer[1])/32.0;
//	int16_t temp_int;
//	if(temp_uint > 1023){
//		temp_int = temp_uint - 2048;
//	}else{
//		temp_int = temp_uint;
//	}
//	sens->temperature = (float)(temp_int)*0.125f + 23.0f;
//}
//
//void set_transformations_acc(struct BMI088_Sens* sens){
//	sens->tr_x[0] = 1; sens->tr_x[1] = 0; sens->tr_x[2] = 0;
//	sens->tr_y[0] = 0; sens->tr_y[1] =-1; sens->tr_y[2] = 0;
//	sens->tr_z[0] = 0; sens->tr_z[1] = 0; sens->tr_z[2] =-1;
//}
//
//uint8_t get_error_acc(struct BMI088_Sens* sens){
//  uint8_t error = 0;
//
//  uint8_t res_get_error = read_reg(sens->i2c, sens->acc_dev_addr, ACC_ERR_CODE_ADDR, &error, 1);
//
//  return GET_FIELD(ACC_ERR_CODE, error);
//}
//uint8_t get_fatal_error_acc(struct BMI088_Sens* sens){
//  uint8_t error = 0;
//  uint8_t res_get_error = read_reg(sens->i2c, sens->acc_dev_addr, ACC_FATAL_ERR_ADDR, &error, 1);
//  return GET_FIELD(ACC_FATAL_ERR, error);
//
//}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
