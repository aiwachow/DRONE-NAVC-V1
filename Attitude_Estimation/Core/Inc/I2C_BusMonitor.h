/*
 * I2C_BusMonitor.h
 *
 *  Created on: Dec 11, 2023
 *      Author: Adam Iwachów
 *
 *     Due to the fact that most of the devices on SensorBoard operate on the I2c bus
 *     a basic I2C Handler was created for errors handle, scanning etc.
 *
 * 		work in progress
 */


#ifndef INC_I2C_BUSMONITOR_H_
#define INC_I2C_BUSMONITOR_H_

#include "i2c.h"
#include "stm32l4xx_hal.h"
#include <stdio.h>

#define I2C_MAX_TIME 100



void ScanI2CBus(I2C_HandleTypeDef *i2cHandle)
{
	 uint8_t MaxNumberOfI2cDevices = (0xFF >> 1);
	 uint8_t counter = 0;
	 HAL_StatusTypeDef Status;

	 printf("Scanning Bus\n");
	 for(uint8_t address = 0; address < MaxNumberOfI2cDevices; ++address)
	 {

	     Status = HAL_I2C_IsDeviceReady(i2cHandle, (uint16_t)(address<<1), 2, I2C_MAX_TIME);
	     if (Status != HAL_OK) // No answer from that address
	     {
	         printf(" - ");
	     }
	     else if(Status == HAL_OK)
	     {
	    	counter++;
	        printf("0x%X", address);
	     }
	 }
	if(counter == 0){
		printf("No devices?\n");
		HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_3);

	}
	printf(" End of scanning, number of devices on Bus: %d\n", counter);
}


void CheckI2Cfaults(uint8_t errors)
{
   if(errors != 0){
	   HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_2);
   }
}


#endif /* INC_I2C_BUSMONITOR_H_ */
