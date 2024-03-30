/*
 * ProcessAltitude.c
 *
 *  Created on: Dec 10, 2023
 *      Author: Adam
 */

#include <Altimeter.h>

static uint8_t PrepareAverageData = 0;

void INIT_ALTIMETER(Altimeter_t *dev, float currpress)
{


	dev->Altitude_1    = 0.0f;
	dev->Altitude_2    = 0.0f;
	dev->AveragePress  = 0.0f;
	dev->AverageTemp   = 0.0f;
	dev->ProcPress 	   = currpress;
	dev->Air_density   = 0.0f;

}

void Estimate_Altitude(Altimeter_t *dev ,SPL06_t *SPL_dev,float CurrentPress, float CurrentTemp, uint8_t TURN_ON)
{

//	 if(PrepareAverageData != TURN_ON){
//		 DataPrepareForAlitmeter(dev,SPL_dev,16.0f);
//		 PrepareAverageData = TURN_ON;
//	 }

	 if(TURN_ON){
		  /* LOW PASS FILTER */
	 dev->ProcPress =  ALPHA * CurrentPress + (1.0f - ALPHA) *  dev->ProcPress;

	 /* BAROMETRIC FORMULA */
	 dev->Altitude_1 = -29.271769 * (CurrentTemp + 273.15) * log(dev->ProcPress / dev->AveragePress);

	 /* HYDROSTATIC FORMULA */
	 // dev->Altitude_2 = -100 *((dev->ProcPress - dev->AveragePress) / (dev->Air_density * GRAVITY_EARTH_STANDARD));

	 }
	 else
	 {
		 dev->Altitude_1 = 0.0f;
	 }


}

void DataPrepareForAlitmeter(Altimeter_t *dev,SPL06_t *SPL_dev, float SAMPLE_NUM)
{
	for(int i = 0; i < SAMPLE_NUM; ++i){
		while(SPL06_PRESS_AND_TEMP_READY(SPL_dev) == 0){;}
	    SPL06_READ_TEMP_PRESS(SPL_dev);

	    dev->AveragePress += SPL_dev->pressure_hPa ;
	    dev->AverageTemp += SPL_dev->temp_cel;
	}

	dev->AveragePress /= SAMPLE_NUM;
	dev->AverageTemp  /= SAMPLE_NUM;

}

void ESTIMATE_AIR_DENSITY(Altimeter_t *dev)
{
	dev->Air_density = 100.0f * (dev->AveragePress / (287.05f *(dev->AverageTemp + 273.15)));
}
