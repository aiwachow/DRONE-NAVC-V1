/*
 * RollPitch.c
 *
 *  Created on: Dec 28, 2023
 *      Author: Adam
 */

#include <RollPitchYaw.h>
#include "math.h"

#define Rad_to_Deg 		57.3248f
#define EarthGravity 	9.81f
/*
 * TO ESTIMATE ROLL AND PITCH ANGLES WE NEED TO DO SOME MATHS
 * we want to extract the ROLL and PITCH angles from both sensors
 * to combine them into a clean measurement with the help of a filter (complementary filter)
 *
 */

/* 					*** ACCELEROMETER ***
 * 1. We must assume the accelerometer remains stationary
 * Stationary accelerometer model:
 * |ax|                                | g * sin(Phi)  			|
 * |ay| = -[RotX * RotY * [0,0,g]^T] = |-g * sin(Ksi) * cos(Phi)|
 * |az|   							   |-g * cos(Phi) * cos(Ksi)|
 * RotZ isn't present since we are ignoring yaw angle
 *
 *  Roll & Pitch:
 *
 *  ax = g * sin(Phi) => Pitch(Phi) = arcsin(ax/g)
 *
 *  ay = -g * sin(Ksi) * cos(Phi)
 *  az = -g * cos(Phi) * cos(Ksi)
 * 	|
 *   \---> ay/az = sin(Ksi)/cos(Ksi) => Roll(Ksi) = arctan(ay/az)
 *
 *
 */
static uint8_t ResetYawAngle = 0;

void InitRollPitchYawEstimationFunc(RollPitchYaw_t *RPY)
{

	RPY->RollAngle  = 0.0f;
	RPY->PitchAngle = 0.0f;
	RPY->YawAngle = 0.0f;

	RPY->RollAngleAcc = 0.0f;
	RPY->PitchAngleAcc = 0.0f;


	RPY->RollAngleGyro = 0.0f;
	RPY->PitchAngleGyro = 0.0f;


	RPY->LP_acc_ms2[0] = 0.0f;
	RPY->LP_acc_ms2[1] = 0.0f;
	RPY->LP_acc_ms2[2] = 0.0f;


	RPY->LP_gyr_degs[0] = 0.0f;
	RPY->LP_gyr_degs[1] = 0.0f;
	RPY->LP_gyr_degs[2] = 0.0f;

	RPY->RollAngleDot  = 0.0f;
	RPY->PitchAngleDot = 0.0f;
	RPY->YawAngleDot = 0.0f;
}


void EstimateRollPitchWithACC(RollPitchYaw_t *RPY, BMI088_t *BMI, float ALPHA_LP)
{



	float prevRoll = 0.0f;
	float prevPitch = 0.0f;


	// Basic I order IIR low-pass filter
	RPY->LP_acc_ms2[0] =  ALPHA_LP * BMI->acc_ms2[0] + (1.0f - ALPHA_LP) *  RPY->LP_acc_ms2[0];
	RPY->LP_acc_ms2[1] =  ALPHA_LP * BMI->acc_ms2[1] + (1.0f - ALPHA_LP) *  RPY->LP_acc_ms2[1];
	RPY->LP_acc_ms2[2] =  ALPHA_LP * BMI->acc_ms2[2] + (1.0f - ALPHA_LP) *  RPY->LP_acc_ms2[2];

	prevRoll  = RPY->RollAngleAcc;
	prevPitch = RPY->PitchAngleAcc;

   RPY->RollAngleAcc  = atan2f(RPY->LP_acc_ms2[1] , RPY->LP_acc_ms2[2]);
   RPY->PitchAngleAcc = asinf(RPY->LP_acc_ms2[0] /  EarthGravity);


   // unwrapping the phase
   if((RPY->RollAngleAcc - prevRoll) > 180.0){
	   RPY->RollAngleAcc-= 360.0;
   }
   else if((RPY->RollAngleAcc - prevRoll) < -180.0){
	   RPY->RollAngleAcc += 360.0;
   }


}

/* 					*** GYROSCOPE ***

 */

void EstimateRollPitchWithGyro(RollPitchYaw_t *RPY, BMI088_t *BMI, float ALPHA_LP, uint16_t SAMPLE_TIME)
{

   // RC filter

	RPY->LP_gyr_degs[0] =  ALPHA_LP* BMI->gyr_degs[0] + (1.0f - ALPHA_LP) *  RPY->LP_gyr_degs[0];
	RPY->LP_gyr_degs[1] =  ALPHA_LP* BMI->gyr_degs[1] + (1.0f - ALPHA_LP) *  RPY->LP_gyr_degs[1];
	RPY->LP_gyr_degs[2] =  ALPHA_LP * BMI->gyr_degs[2] + (1.0f - ALPHA_LP) *  RPY->LP_gyr_degs[2];


    RPY->RollAngleDot  = RPY->LP_gyr_degs[0] + tanf(RPY->PitchAngleGyro)*(RPY->LP_gyr_degs[1] * sinf( RPY->RollAngleGyro ) + RPY->LP_gyr_degs[2] * cosf(RPY->RollAngleGyro));
    RPY->PitchAngleDot = RPY->LP_gyr_degs[1] * cosf(RPY->RollAngleGyro) - RPY->LP_gyr_degs[2] * sinf( RPY->RollAngleGyro );

    RPY->RollAngleGyro  = RPY->RollAngleGyro  + (SAMPLE_TIME/1000.0f) * RPY->RollAngleDot;
    RPY->PitchAngleGyro = RPY->PitchAngleGyro + (SAMPLE_TIME/1000.0f) * RPY->PitchAngleDot;

}



static void EstimateEulerRPYAngle(RollPitchYaw_t *RPY, float ax, float ay, float az)
{
	 if(az == 0.0f){
		 // poor protection against going angles out of range ( Gimbal lock)
			   az = 0.01f;
	   }
	RPY->RollAngleAcc  = atan2f(ay , az);
	RPY->PitchAngleAcc = asinf(ax /  EarthGravity);
}

static void EstimateEulerRPYAngleDot(RollPitchYaw_t *RPY, float p, float q, float r)
{
   if(cosf(RPY->PitchAngle) == 0.0f || sinf(RPY->PitchAngle) == 0.0f ){
	   // poor protection against going angles out of range
		   RPY->PitchAngle = 0.01f;
   }
   RPY->RollAngleDot  = p + tanf(RPY->PitchAngle)*(q * sinf( RPY->RollAngle) + r * cosf(RPY->RollAngle));
   RPY->PitchAngleDot = q * cosf(RPY->RollAngle) - r * sinf( RPY->RollAngle);
   RPY->YawAngleDot   = q * (sinf(RPY->RollAngle)*(1/cosf(RPY->PitchAngle))) + r* (cosf(RPY->RollAngle)*(1/cosf(RPY->PitchAngle)) );
}


void EstimateRollPitchYawComplementaryFilter(RollPitchYaw_t *RPY, BMI088_t *BMI,QMC5883L_t *QMC, float ALPHA_ACC,
		float ALPHA_GYRO, uint16_t SAMPLE_TIME, float ALPHA_COMPLEMENTARY, uint8_t MAGNETOMETER_IS_ON)
{

/* Basic I order IIR low-pass filter */

RPY->LP_acc_ms2[0] = ALPHA_ACC * BMI->acc_ms2[0] + (1.0f - ALPHA_ACC) *  RPY->LP_acc_ms2[0];
RPY->LP_acc_ms2[1] = ALPHA_ACC * BMI->acc_ms2[1] + (1.0f - ALPHA_ACC) *  RPY->LP_acc_ms2[1];
RPY->LP_acc_ms2[2] = ALPHA_ACC * BMI->acc_ms2[2] + (1.0f - ALPHA_ACC) *  RPY->LP_acc_ms2[2];

// Be careful with filtering gryo data, gyro carry high freq information so keep ALPHA_GYRO close to 1
RPY->LP_gyr_degs[0] = ALPHA_GYRO * BMI->gyr_degs[0] + (1.0f - ALPHA_GYRO) *  RPY->LP_gyr_degs[0];
RPY->LP_gyr_degs[1] = ALPHA_GYRO * BMI->gyr_degs[1] + (1.0f - ALPHA_GYRO) *  RPY->LP_gyr_degs[1];
RPY->LP_gyr_degs[2] = ALPHA_GYRO * BMI->gyr_degs[2] + (1.0f - ALPHA_GYRO) *  RPY->LP_gyr_degs[2];



/* Angles estimation by accelerometer */
EstimateEulerRPYAngle(RPY, RPY->LP_acc_ms2[0], RPY->LP_acc_ms2[1],RPY->LP_acc_ms2[2]);

/* Speed of change of angles, gyroscope */
EstimateEulerRPYAngleDot(RPY, RPY->LP_gyr_degs[0], RPY->LP_gyr_degs[1], RPY->LP_gyr_degs[2]);


/* Roll & Pitch & Yaw */
RPY->RollAngle  = ALPHA_COMPLEMENTARY * RPY->RollAngleAcc  + (1.0f - ALPHA_COMPLEMENTARY) * (RPY->RollAngle
		+   ((float)SAMPLE_TIME / 1000.0f) * RPY->RollAngleDot);

RPY->PitchAngle = ALPHA_COMPLEMENTARY * RPY->PitchAngleAcc + (1.0f - ALPHA_COMPLEMENTARY) * (RPY->PitchAngle
		+ ((float)SAMPLE_TIME / 1000.0f) * RPY->PitchAngleDot);


/* due to unpredictable magnetometer behaviour there is option to measure yaw only with gyro */
if(MAGNETOMETER_IS_ON){
		 if(ResetYawAngle != MAGNETOMETER_IS_ON){
				RPY->YawAngle = 0.0f;
				ResetYawAngle = MAGNETOMETER_IS_ON;
		 }


	// fit mag data to euler rate
	RPY->Xmag = -cosf(RPY->RollAngle)*QMC->CalMagData[1] + sinf(RPY->RollAngle)*QMC->CalMagData[2];

	RPY->Ymag = QMC->CalMagData[0]*cosf(RPY->PitchAngle) + (sinf(RPY->PitchAngle)*sinf(RPY->RollAngle))*QMC->CalMagData[1]
			+ (sinf(RPY->PitchAngle)*cosf(RPY->RollAngle))*QMC->CalMagData[2];

	if(RPY->Xmag == 0){
		RPY->Xmag = 0.01f;
	}

	RPY->YawAngle = atan2f(RPY->Xmag, RPY->Ymag) + (6.0f * 3.14/180);
	RPY->YawAngle = ALPHA_COMPLEMENTARY * RPY->YawAngleMag  + (1.0f - ALPHA_COMPLEMENTARY) * (RPY->YawAngle
			+   ((float)SAMPLE_TIME / 1000.0f) * RPY->YawAngleDot);
}
else{

	RPY->YawAngle = (RPY->YawAngle  + ((float)SAMPLE_TIME / 1000.0f) * RPY->YawAngleDot);

}



}

