/*
 * RollPitch.h
 *
 *  Created on: Dec 28, 2023
 *      Author: Adam Iwachów
 *
 *      At this moment: RollPitchYaw.h includes only funcs like complementary filter for estimate Euler agnles,
 *      In future: Replace Euler to quaternions, basic complementary to cascade or kalman
 */

#ifndef INC_ROLLPITCH_H_
#define INC_ROLLPITCH_H_


#include <BMI088.h>
#include "QMC5883L.h"


typedef struct{

    float RollAngle;
    float PitchAngle;
    float YawAngle;


    float RollAngleAcc;
    float PitchAngleAcc;

    float RollAngleGyro;
    float PitchAngleGyro;

    float YawAngleMag;

	float LP_acc_ms2[3];
	float LP_gyr_degs[3];

	float RollAngleDot;
	float PitchAngleDot;
	float YawAngleDot;

	float Xmag, Ymag;


}RollPitchYaw_t;

void InitRollPitchYawEstimationFunc(RollPitchYaw_t *RPY);
void EstimateRollPitchWithACC(RollPitchYaw_t *RPY, BMI088_t *BMI, float ALPHA_RPY);
void EstimateRollPitchWithGyro(RollPitchYaw_t *RPY, BMI088_t *BMI, float ALPHA_RPY, uint16_t SAMPLE_TIME);
void EstimateRollPitchYawComplementaryFilter(RollPitchYaw_t *RPY, BMI088_t *BMI,QMC5883L_t *QMC, float ALPHA_ACC,
float ALPHA_GYRO, uint16_t SAMPLE_TIME, float ALPHA_COMPLEMENTARY, uint8_t MAGNETOMETER_IS_ON);


#endif /* INC_ROLLPITCH_H_ */
