/*
 * madgwick.hpp
 *
 *  Created on: Apr 8, 2023
 *      Author: leo
 */

#ifndef MADGWICK_CORE_INC_MADGWICK_HPP_
#define MADGWICK_CORE_INC_MADGWICK_HPP_


#include "DataStructures.h"
#include "bmi088_sens.hpp"
#include "lis3mdl_sens.hpp"

Quaternion madgwick_quat_upd(BMI088_Sens::xyz acc, BMI088_Sens::xyz gyr, LIS3MDL_Sens::xyz mag, const float& deltat, Quaternion& q, float beta);
#endif /* MADGWICK_CORE_INC_MADGWICK_HPP_ */
