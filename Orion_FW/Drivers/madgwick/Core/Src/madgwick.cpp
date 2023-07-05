/*
 * madgwick.cpp
 *
 *  Created on: Apr 8, 2023
 *      Author: leo
 */

#include "madgwick.hpp"

Quaternion madgwick_quat_upd(BMI088_Sens::xyz acc, BMI088_Sens::xyz gyr, LIS3MDL_Sens::xyz mag, const float& deltat, Quaternion& q, float beta=1.5f){

	float q1 = q.w;
    float q2 = q.x;
    float q3 = q.y;
    float q4 = q.z;   // short name local variable for readability
    float norm;
    float hx, hy, _2bx, _2bz;
    float s1, s2, s3, s4;
    float qDot1, qDot2, qDot3, qDot4;

    // Auxiliary variables to avoid repeated arithmetic
    float _2q1mx;
    float _2q1my;
    float _2q1mz;
    float _2q2mx;
    float _4bx;
    float _4bz;
    float _2q1 = 2.0f * q1;
    float _2q2 = 2.0f * q2;
    float _2q3 = 2.0f * q3;
    float _2q4 = 2.0f * q4;
    float _2q1q3 = 2.0f * q1 * q3;
    float _2q3q4 = 2.0f * q3 * q4;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q1q4 = q1 * q4;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q2q4 = q2 * q4;
    float q3q3 = q3 * q3;
    float q3q4 = q3 * q4;
    float q4q4 = q4 * q4;

    // Normalise accelerometer measurement
    norm = 1.0f / sqrtf(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
    acc.x *= norm;
    acc.y *= norm;
    acc.z *= norm;

    // Normalise magnetometer measurement
    norm = 1.0f / sqrtf(mag.x * mag.x + mag.y * mag.y + mag.z * mag.z);
    mag.x *= norm;
    mag.y *= norm;
    mag.z *= norm;

    // Reference direction of Earth's magnetic field
    _2q1mx = 2.0f * q1 * mag.x;
    _2q1my = 2.0f * q1 * mag.y;
    _2q1mz = 2.0f * q1 * mag.z;
    _2q2mx = 2.0f * q2 * mag.x;
    hx = mag.x * q1q1 - _2q1my * q4 + _2q1mz * q3 + mag.x * q2q2 + _2q2 * mag.y * q3 + _2q2 * mag.z * q4 - mag.x * q3q3 - mag.x * q4q4;
    hy = _2q1mx * q4 + mag.y * q1q1 - _2q1mz * q2 + _2q2mx * q3 - mag.y * q2q2 + mag.y * q3q3 + _2q3 * mag.z * q4 - mag.y * q4q4;
    _2bx = sqrtf(hx * hx + hy * hy);
    _2bz = -_2q1mx * q3 + _2q1my * q2 + mag.z * q1q1 + _2q2mx * q4 - mag.z * q2q2 + _2q3 * mag.y * q4 - mag.z * q3q3 + mag.z * q4q4;
    _4bx = 2.0f * _2bx;
    _4bz = 2.0f * _2bz;

    // Gradient decent algorithm corrective step
    s1 = -_2q3 * (2.0f * q2q4 - _2q1q3 - acc.x) + _2q2 * (2.0f * q1q2 + _2q3q4 - acc.y) - _2bz * q3 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mag.x) + (-_2bx * q4 + _2bz * q2) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - mag.y) + _2bx * q3 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mag.z);
    s2 = _2q4 * (2.0f * q2q4 - _2q1q3 - acc.x) + _2q1 * (2.0f * q1q2 + _2q3q4 - acc.y) - 4.0f * q2 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - acc.z) + _2bz * q4 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mag.x) + (_2bx * q3 + _2bz * q1) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - mag.y) + (_2bx * q4 - _4bz * q2) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mag.z);
    s3 = -_2q1 * (2.0f * q2q4 - _2q1q3 - acc.x) + _2q4 * (2.0f * q1q2 + _2q3q4 - acc.y) - 4.0f * q3 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - acc.z) + (-_4bx * q3 - _2bz * q1) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mag.x) + (_2bx * q2 + _2bz * q4) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - mag.y) + (_2bx * q1 - _4bz * q3) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mag.z);
    s4 = _2q2 * (2.0f * q2q4 - _2q1q3 - acc.x) + _2q3 * (2.0f * q1q2 + _2q3q4 - acc.y) + (-_4bx * q4 + _2bz * q2) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mag.x) + (-_2bx * q1 + _2bz * q3) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - mag.y) + _2bx * q2 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mag.z);
    norm = sqrtf(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4);    // normalise step magnitude
    norm = 1.0f/norm;
    s1 *= norm;
    s2 *= norm;
    s3 *= norm;
    s4 *= norm;

    // Compute rate of change of quaternion
    qDot1 = 0.5f * (-q2 * gyr.x - q3 * gyr.y - q4 * gyr.z) - beta * s1;
    qDot2 = 0.5f * (q1 * gyr.x + q3 * gyr.z - q4 * gyr.y) - beta * s2;
    qDot3 = 0.5f * (q1 * gyr.y - q2 * gyr.z + q4 * gyr.x) - beta * s3;
    qDot4 = 0.5f * (q1 * gyr.z + q2 * gyr.y - q3 * gyr.x) - beta * s4;

    // Integrate to yield quaternion
    q1 += qDot1 * deltat;
    q2 += qDot2 * deltat;
    q3 += qDot3 * deltat;
    q4 += qDot4 * deltat;
    norm = sqrtf(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);    // normalise quaternion
    norm = 1.0f/norm;
    q.w = q1 * norm;
    q.x = q2 * norm;
    q.y = q3 * norm;
    q.z = q4 * norm;

    return q;
}
