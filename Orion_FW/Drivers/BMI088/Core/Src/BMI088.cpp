/*
 * BMI088.cpp
 *
 *      Author: Vincent Nguyen
 */

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include <BMI088.hpp>
#include <BMI088_defs.hpp>

HAL_StatusTypeDef BMI088::read_reg(uint8_t reg, uint8_t* data, uint16_t len, uint8_t dev_addr) {
//  not sure if necessary to suspend tasks and disable interrupts
//	vTaskSuspendAll();
//	__disable_irq();
	HAL_StatusTypeDef res = HAL_I2C_Mem_Read(this->i2c, dev_addr << 1, reg, I2C_MEMADD_SIZE_8BIT, data, len, 1000);
//	__enable_irq();
//	xTaskResumeAll();
	if (res != HAL_OK) {
		for (uint16_t i = 0; i < len; ++ i)
			data[i] = 0;
	}
	return res;
}

HAL_StatusTypeDef BMI088::read_bits(uint8_t reg, uint8_t* data, uint8_t bits, uint8_t shift, uint8_t dev_addr) {
    if (data == nullptr || bits > 8 || shift > 7) {
        return HAL_ERROR; // Invalid parameters
    }

    uint8_t reg_value;
    HAL_StatusTypeDef res;

    res = read_reg(reg, &reg_value, 1, dev_addr);
    if (res != HAL_OK) {
        return res; // Error occurred during the I2C read
    }

    // Mask the bits we want to keep (set the other bits to 0)
    uint8_t mask = (1 << bits) - 1;

    // Shift and extract the desired bits
    *data = (reg_value >> shift) & mask;

    return HAL_OK;
}

HAL_StatusTypeDef BMI088::write_reg(uint8_t reg, uint8_t data, uint8_t dev_addr) {
//  not sure if necessary, to test
//	vTaskSuspendAll();
//	__disable_irq();
	HAL_StatusTypeDef res = HAL_I2C_Mem_Write(this->i2c, dev_addr << 1, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
//	__enable_irq();
//	xTaskResumeAll();



	return res;
}

HAL_StatusTypeDef BMI088::initialize_accel(){

	HAL_StatusTypeDef status;

	// Startup sequence

	status = chip_id_ok_acc();
	if (status != HAL_OK)
		return status;

	status = soft_reset_acc();
	if (status != HAL_OK)
		return status;

	osDelay(100);
  
	status = set_power_acc(ACC_ENABLE);
	if (status != HAL_OK)
		return status;

	osDelay(100);

	status = set_mode_acc(ACC_ACTIVE);
	if (status != HAL_OK)
		return status;

	osDelay(100);

	status = self_test_acc();
    if (status != HAL_OK)
		return status;

    osDelay(100);

	status = soft_reset_acc();
	if (status != HAL_OK)
		return status;

	osDelay(100);

	status = set_power_acc(ACC_ENABLE);
	if (status != HAL_OK)
		return status;

	osDelay(100);

	status = set_mode_acc(ACC_ACTIVE);
	if (status != HAL_OK)
		return status;

	osDelay(100);

	// Load configuration parameters

	status = set_range_acc(config.range_acc_conf);
	if (status != HAL_OK)
		return status;

	status = set_odr_acc(this->config.odr_acc_conf);
	if (status != HAL_OK)
		return status;

	status = check_config_err_acc();
	if (status != HAL_OK)
		return status;

	status = check_fatal_err_acc();
	if (status != HAL_OK)
		return status;

	return HAL_OK;
}

HAL_StatusTypeDef BMI088::initialize_gyro(){

	HAL_StatusTypeDef status;

	// Startup sequence

	status = chip_id_ok_gyro();
	if (status != HAL_OK)
		return status;

	// For some reason there's an error writing config after reset
//	status = soft_reset_gyro();
//	if (status != HAL_OK)
//		return status;

	status = set_range_gyro(config.range_gyro_conf);
	if (status != HAL_OK)
		return status;


	status = set_drdy_int_gyro(GYRO_ENABLE_DRDY_INT);
	if (status != HAL_OK)
		return status;

	status = set_odr_gyro(config.odr_gyro_conf);
	if (status != HAL_OK)
		return status;

	return HAL_OK;
}

HAL_StatusTypeDef BMI088::chip_id_ok_gyro(){
	HAL_StatusTypeDef status;
	uint8_t chip_id = 0;
	status = read_reg(GYRO_CHIP_ID_ADDR, &chip_id, 1, gyro_dev_addr);
	if (status != HAL_OK)
		return status;

	return (chip_id == GYRO_CHIP_ID)? HAL_OK : HAL_ERROR;
}

HAL_StatusTypeDef BMI088::chip_id_ok_acc(){
	HAL_StatusTypeDef status;
	uint8_t chip_id = 0;
	status = read_reg(BMI08_REG_ACCEL_CHIP_ID, &chip_id, 1, acc_dev_addr);
	if (status != HAL_OK)
		return status;

	return (chip_id == ACC_CHIP_ID)? HAL_OK : HAL_ERROR;
}


HAL_StatusTypeDef BMI088::check_config_err_acc() {
	HAL_StatusTypeDef status;
	uint8_t err = 0;
	status = read_bits(ACC_ERR_CODE_ADDR, &err, 3, 2, acc_dev_addr);
	if (status != HAL_OK)
		return status;

	if (err == 0x00)
		return HAL_OK;
	else
		return HAL_ERROR;
}


HAL_StatusTypeDef BMI088::check_fatal_err_acc() {
	HAL_StatusTypeDef status;
	uint8_t err = 0;
	status = read_bits(ACC_ERR_CODE_ADDR, &err, 1, 0, acc_dev_addr);
	if (status != HAL_OK)
		return status;

	if (err == 0x00)
		return HAL_OK;
	else
		return HAL_ERROR;
}

HAL_StatusTypeDef BMI088::soft_reset_acc(){
	return write_reg(ACC_SOFT_RESET_ADDR, (uint8_t)ACC_RESET_CMD, acc_dev_addr);
}


HAL_StatusTypeDef BMI088::set_range_acc(acc_scale_type_t range){
	HAL_StatusTypeDef status;
	status = write_reg(ACC_RANGE_ADDR, (uint8_t)range, acc_dev_addr);
	if (status != HAL_OK)
		return status;

	switch (range) {
	case RANGE_3G:
		acc_range = 3;
		break;
	case RANGE_6G:
		acc_range = 6;
		break;
	case RANGE_12G:
		acc_range = 12;
		break;
	case RANGE_24G:
		acc_range = 24;
		break;
	}
	return HAL_OK;
}

HAL_StatusTypeDef BMI088::set_odr_acc(acc_odr_type_t odr){
	return write_reg(ACC_ODR_ADDR, (uint8_t)odr, acc_dev_addr);
}

HAL_StatusTypeDef BMI088::set_power_acc(acc_power_type_t power){
	return write_reg(ACC_POWER_CNTRL_ADDR, (uint8_t)power, acc_dev_addr);
}

HAL_StatusTypeDef BMI088::set_mode_acc(acc_mode_type_t type){
	return write_reg(ACC_POWER_CONF_ADDR, (uint8_t)type, acc_dev_addr);
}

HAL_StatusTypeDef BMI088::self_test_acc(){
	HAL_StatusTypeDef status;
	Vector accel_pos_mg;
	Vector accel_neg_mg;

	status = set_range_acc(RANGE_24G);
	if (status != HAL_OK)
		return status;

	status = set_odr_acc(ODR_1600HZ_BW_145HZ);
	if (status != HAL_OK)
		return status;

	osDelay(3);

	status = write_reg(ACC_SELF_TEST_ADDR, (uint8_t)ACC_POS_SELF_TEST, acc_dev_addr);
	if (status != HAL_OK)
		return status;

	osDelay(51);

	status = get_accel(accel_pos_mg);
	if (status != HAL_OK)
		return status;

	accel_pos_mg.x *= 1000.0f/G;
	accel_pos_mg.y *= 1000.0f/G;
	accel_pos_mg.z *= 1000.0f/G;

	status = write_reg(ACC_SELF_TEST_ADDR, (uint8_t)ACC_NEG_SELF_TEST, acc_dev_addr);
	if (status != HAL_OK)
		return status;

	osDelay(51);

	status = get_accel(accel_neg_mg);
	if (status != HAL_OK)
		return status;

	accel_neg_mg.x *= 1000.0f/G;
	accel_neg_mg.y *= 1000.0f/G;
	accel_neg_mg.z *= 1000.0f/G;

	status = write_reg(ACC_SELF_TEST_ADDR, (uint8_t)ACC_DIS_SELF_TEST, acc_dev_addr);
	if (status != HAL_OK)
		return status;

	if((fabs(accel_pos_mg.x - accel_neg_mg.x) >= 1000)
		&& (fabs(accel_pos_mg.y - accel_neg_mg.y) >=1000)
		&& (fabs(accel_pos_mg.z - accel_neg_mg.z)>=500)) {
		return HAL_OK;
	} else {
		return HAL_ERROR;
	}
}

HAL_StatusTypeDef BMI088::read_accel() {
	HAL_StatusTypeDef status;
	uint8_t buff[6];
	status = read_reg(ACC_ACCEL_DATA_ADDR, buff, 6, acc_dev_addr);
	if (status != HAL_OK) {
		return status;
	}

	// Read raw values in two's complement

	acc.x = (int16_t)((buff[1] << 8 ) | buff[0]);
	acc.y = (int16_t)((buff[3] << 8 ) | buff[2]);
	acc.z = (int16_t)((buff[5] << 8 ) | buff[4]);

	// Convert to m/s^2

	acc_mss.x = acc.x / 32767.0f * acc_range * G;
	acc_mss.y = acc.y / 32767.0f * acc_range * G;
	acc_mss.z = acc.z / 32767.0f * acc_range * G;

	// Remove bias and apply transform
	float x_nb = acc_mss.x - ACC_BIAS[0];
	float y_nb = acc_mss.y - ACC_BIAS[1];
	float z_nb = acc_mss.z - ACC_BIAS[2];

	acc_cal_mss.x = ACC_TF[0][0]*x_nb + ACC_TF[0][1]*y_nb + ACC_TF[0][2]*z_nb;
	acc_cal_mss.y = ACC_TF[1][0]*x_nb + ACC_TF[1][1]*y_nb + ACC_TF[1][2]*z_nb;
	acc_cal_mss.z = ACC_TF[2][0]*x_nb + ACC_TF[2][1]*y_nb + ACC_TF[2][2]*z_nb;

	return status;
}

HAL_StatusTypeDef BMI088::get_accel(Vector& acc_){
	HAL_StatusTypeDef status;
	status = read_accel();
	if (status != HAL_OK) {
		acc_.x = 0;
		acc_.y = 0;
		acc_.z = 0;
		return status;
	}
	acc_.x = acc_mss.x;
	acc_.y = acc_mss.y;
	acc_.z = acc_mss.z;
	return HAL_OK;
}

HAL_StatusTypeDef BMI088::get_accel_cal(Vector& acc_){
	HAL_StatusTypeDef status;
	status = read_accel();
	if (status != HAL_OK) {
		acc_.x = 0;
		acc_.y = 0;
		acc_.z = 0;
		return status;
	}
	acc_.x = acc_cal_mss.x;
	acc_.y = acc_cal_mss.y;
	acc_.z = acc_cal_mss.z;
	return HAL_OK;
}

HAL_StatusTypeDef BMI088::get_accel_raw(Vector& acc_){
	HAL_StatusTypeDef status;
	status = read_accel();
	if (status != HAL_OK) {
		acc_.x = 0;
		acc_.y = 0;
		acc_.z = 0;
		return status;
	}
	acc_.x = acc.x;
	acc_.y = acc.y;
	acc_.z = acc.z;
	return HAL_OK;
}

HAL_StatusTypeDef BMI088::soft_reset_gyro(){
	HAL_StatusTypeDef status;
	status = write_reg(GYRO_SOFT_RESET_ADDR, (uint8_t)GYRO_RESET_CMD, gyro_dev_addr);
	if (status != HAL_OK)
		return status;
	osDelay(50);
	return HAL_OK;
}

HAL_StatusTypeDef BMI088::set_range_gyro(gyro_scale_type_t range){
	HAL_StatusTypeDef status;
	status = write_reg(GYRO_RANGE_ADDR, (uint8_t)range, gyro_dev_addr);
	if (status != HAL_OK)
		return status;

	switch (range) {
	case RANGE_2000:
		gyro_range = 2000;
		break;
	case RANGE_1000:
		gyro_range = 1000;
		break;
	case RANGE_500:
		gyro_range = 500;
		break;
	case RANGE_250:
		gyro_range = 250;
		break;
	case RANGE_125:
		gyro_range = 125;
		break;
	}

	return HAL_OK;
}

HAL_StatusTypeDef BMI088::set_drdy_int_gyro(gyro_drdy_int_type_t type){
	return write_reg(GYRO_INT_CNTRL_ADDR, (uint8_t)type, gyro_dev_addr);
}

HAL_StatusTypeDef BMI088::set_odr_gyro(gyro_odr_type_t odr){
	return write_reg(GYRO_ODR_ADDR, (uint8_t)odr, gyro_dev_addr);
}

HAL_StatusTypeDef BMI088::read_gyro() {
	HAL_StatusTypeDef status;
	uint8_t buff[6];

	status = read_reg(GYRO_DATA_ADDR, buff, 6, gyro_dev_addr);
	if (status != HAL_OK) {
		return status;
	}

	// Read raw values in two's complement and convert to float

	gyro.x = (int16_t)((buff[1] << 8 ) | buff[0]);
	gyro.y = (int16_t)((buff[3] << 8 ) | buff[2]);
	gyro.z = (int16_t)((buff[5] << 8 ) | buff[4]);

	// Convert to rad/s

	gyro_rads.x = gyro.x / 32767.0f * gyro_range * D2R;
	gyro_rads.y = gyro.y / 32767.0f * gyro_range * D2R;
	gyro_rads.z = gyro.z / 32767.0f * gyro_range * D2R;

	// Remove bias

	gyro_cal_rads.x = gyro_rads.x - GYRO_BIAS[0];
	gyro_cal_rads.y = gyro_rads.y - GYRO_BIAS[1];
	gyro_cal_rads.z = gyro_rads.z - GYRO_BIAS[2];

	return status;
}

HAL_StatusTypeDef BMI088::get_gyro(Vector& gyro_){
	HAL_StatusTypeDef status;
	status = read_gyro();
	if (status != HAL_OK) {
		gyro_.x = 0;
		gyro_.y = 0;
		gyro_.z = 0;
		return status;
	}
	gyro_.x = gyro_rads.x;
	gyro_.y = gyro_rads.y;
	gyro_.z = gyro_rads.z;
	return HAL_OK;
}

HAL_StatusTypeDef BMI088::get_gyro_cal(Vector& gyro_){
	HAL_StatusTypeDef status;
	status = read_gyro();
	if (status != HAL_OK) {
		gyro_.x = 0;
		gyro_.y = 0;
		gyro_.z = 0;
		return status;
	}
	gyro_.x = gyro_cal_rads.x;
	gyro_.y = gyro_cal_rads.y;
	gyro_.z = gyro_cal_rads.z;
	return HAL_OK;
}

HAL_StatusTypeDef BMI088::get_gyro_raw(Vector& gyro_){
	HAL_StatusTypeDef status;
	status = read_gyro();
	if (status != HAL_OK) {
		gyro_.x = 0;
		gyro_.y = 0;
		gyro_.z = 0;
		return status;
	}
	gyro_.x = gyro.x;
	gyro_.y = gyro.y;
	gyro_.z = gyro.z;
	return HAL_OK;
}

HAL_StatusTypeDef BMI088::compute_gyro_bias(Vector& bias_, uint32_t times) {
	HAL_StatusTypeDef status;
	uint8_t delay = 0;
	switch(config.odr_gyro_conf) {
	case ODR_2000HZ_BW_532:
	case ODR_2000HZ_BW_230:
		delay = 0;
		break;
	case ODR_1000HZ_BW_116:
		delay = 1;
		break;
	case ODR_400HZ_BW_47:
		delay = 2;
		break;
	case ODR_200HZ_BW_23:
	case ODR_200HZ_BW_64:
		delay = 4;
		break;
	case ODR_100HZ_BW_12:
	case ODR_100HZ_BW_32:
		delay = 8;
		break;
	}

	double sum_x = 0;
	double sum_y = 0;
	double sum_z = 0;
	for (uint32_t i = 0; i < times; ++i) {
		status = read_gyro();
		if (status != HAL_OK)
			return status;

		sum_x += (double)gyro_rads.x;
		sum_y += (double)gyro_rads.y;
		sum_z += (double)gyro_rads.z;
		osDelay(delay);
	}
	bias_.x = sum_x/times;
	bias_.y = sum_y/times;
	bias_.z = sum_z/times;
	return HAL_OK;
}

void BMI088::set_bias_gyro(float bias[3]) {
	for (uint8_t i = 0; i < 3; ++i) {
		GYRO_BIAS[i] = bias[i];
	}
}

void BMI088::set_bias_accel(float bias[3]) {
	for (uint8_t i = 0; i < 3; ++i) {
		ACC_BIAS[i] = bias[i];
	}
}

void BMI088::set_transform_accel(float transform[9]) {
	ACC_TF.clear();
    for (uint8_t i = 0; i < 3; i++) {
        std::vector<float> row;
        for (uint8_t j = 0; j < 3; j++) {
            row.push_back(transform[i * 3 + j]);
        }
        ACC_TF.push_back(row);
    }
}

const float* BMI088::get_bias_gyro() const {
	return GYRO_BIAS;
}

const float* BMI088::get_bias_accel() const {
	return ACC_BIAS;
}

const float* BMI088::get_transform_accel() const {
	float* transform = new float[9];
	int index = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			transform[index++] = ACC_TF[i][j];
		}
	}
	return transform;
}

