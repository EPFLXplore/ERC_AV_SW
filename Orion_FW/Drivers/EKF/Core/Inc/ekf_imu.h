/*
 * ekf_imu.h
 *
 *  Created on: May 20, 2023
 *      Author: leo
 */

#ifndef EKF_CORE_INC_EKF_IMU_H_
#define EKF_CORE_INC_EKF_IMU_H_
//
//#include "matrix.h"
////#include "ekf.h"
//
///* ================================================== The AHRS/IMU variables ================================================== */
///* Gravity vector constant (align with global Z-axis) */
//#define IMU_ACC_Z0          (1)
///* Magnetic vector constant (align with local magnetic vector) */
//float_prec IMU_MAG_B0_data[3] = {cos(0), sin(0), 0.000000};
//Matrix IMU_MAG_B0(3, 1, IMU_MAG_B0_data);
///* The hard-magnet bias */
//float_prec HARD_IRON_BIAS_data[3] = {8.832973, 7.243323, 23.95714};
//Matrix HARD_IRON_BIAS(3, 1, HARD_IRON_BIAS_data);
//
//class EKF;
//
///* ============================================ EKF variables/function declaration ============================================ */
///* Just example; in konfig.h:
// *  SS_X_LEN = 4
// *  SS_Z_LEN = 6
// *  SS_U_LEN = 3
// */
///* EKF initialization constant -------------------------------------------------------------------------------------- */
//#define P_INIT      (10.)
//#define Q_INIT      (1e-6)
//#define R_INIT_ACC  (0.0015/10.)
//#define R_INIT_MAG  (0.0015/10.)
///* P(k=0) variable -------------------------------------------------------------------------------------------------- */
//float_prec EKF_PINIT_data[SS_X_LEN*SS_X_LEN] = {P_INIT, 0,      0,      0,
//                                                0,      P_INIT, 0,      0,
//                                                0,      0,      P_INIT, 0,
//                                                0,      0,      0,      P_INIT};
//Matrix EKF_PINIT(SS_X_LEN, SS_X_LEN, EKF_PINIT_data);
///* Q constant ------------------------------------------------------------------------------------------------------- */
//float_prec EKF_QINIT_data[SS_X_LEN*SS_X_LEN] = {Q_INIT, 0,      0,      0,
//                                                0,      Q_INIT, 0,      0,
//                                                0,      0,      Q_INIT, 0,
//                                                0,      0,      0,      Q_INIT};
//Matrix EKF_QINIT(SS_X_LEN, SS_X_LEN, EKF_QINIT_data);
///* R constant ------------------------------------------------------------------------------------------------------- */
//float_prec EKF_RINIT_data[SS_Z_LEN*SS_Z_LEN] = {R_INIT_ACC, 0,          0,          0,          0,          0,
//                                                0,          R_INIT_ACC, 0,          0,          0,          0,
//                                                0,          0,          R_INIT_ACC, 0,          0,          0,
//                                                0,          0,          0,          R_INIT_MAG, 0,          0,
//                                                0,          0,          0,          0,          R_INIT_MAG, 0,
//                                                0,          0,          0,          0,          0,          R_INIT_MAG};
//Matrix EKF_RINIT(SS_Z_LEN, SS_Z_LEN, EKF_RINIT_data);
///* Nonlinear & linearization function ------------------------------------------------------------------------------- */
//bool Main_bUpdateNonlinearX(Matrix& X_Next, const Matrix& X, const Matrix& U);
//bool Main_bUpdateNonlinearY(Matrix& Y, const Matrix& X, const Matrix& U);
//bool Main_bCalcJacobianF(Matrix& F, const Matrix& X, const Matrix& U);
//bool Main_bCalcJacobianH(Matrix& H, const Matrix& X, const Matrix& U);
///* EKF variables ---------------------------------------------------------------------------------------------------- */
//Matrix quaternionData(SS_X_LEN, 1);
//Matrix Y(SS_Z_LEN, 1);
//Matrix U(SS_U_LEN, 1);
///* EKF system declaration ------------------------------------------------------------------------------------------- */
//EKF EKF_IMU(quaternionData, EKF_PINIT, EKF_QINIT, EKF_RINIT,
//            Main_bUpdateNonlinearX, Main_bUpdateNonlinearY, Main_bCalcJacobianF, Main_bCalcJacobianH);
//
//



#endif /* EKF_CORE_INC_EKF_IMU_H_ */
