/*
 * ProcessAltitude.h
 *
 *  Created on: Dec 10, 2023
 *      Author: Adam Iwachów
 *
 *      Basic funcs to estimate altitude from pressure sensor
 */

#ifndef INC_ALTIMETER_H_
#define INC_ALTIMETER_H_

#include <math.h>
#include <SPL06.h>


#define ALPHA 0.1

#define DENSITY_AIR_STANDARD 1.225
#define GRAVITY_EARTH_STANDARD 9.81

typedef struct{

	float Altitude_1;
	float Altitude_2;
	float Air_density;

	float AveragePress;
	float AverageTemp;
	float ProcPress;


}Altimeter_t;


void INIT_ALTIMETER(Altimeter_t *dev,float currpress);
void DataPrepareForAlitmeter(Altimeter_t *dev,SPL06_t *SPL_dev, float SAMPLE_NUM);
void Estimate_Altitude(Altimeter_t *dev ,SPL06_t *SPL_dev,float CurrentPress, float CurrentTemp, uint8_t TURN_ON);
void ESTIMATE_AIR_DENSITY(Altimeter_t *dev);

#endif /* INC_ALTIMETER_H_ */
