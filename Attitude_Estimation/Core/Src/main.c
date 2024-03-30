/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "I2C_BusMonitor.h"
#include "Altimeter.h"
#include "QMC5883L.h"
#include "BMI088.h"
#include "LPS25HB_I2C.h"
#include "ICM_42688_P.h"
#include "SPL06.h"
#include "RollPitchYaw.h"
#include <stdio.h>
#include <SPL06.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SAMPLE_RATE 50
#define MAGNETOMETER_ON   1
#define MAGNETOMETER_OFF  2
#define ALTIMETER_ON  	  2
#define ALTIMETER_OFF     1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

typedef struct{

	LPS25HB_t 	LPS25;
	SPL06_t   	SPL;
	Altimeter_t Altimeter;
	ICM_42688_t IMC;
	BMI088_t 	BMI;
	QMC5883L_t 	QMC;

}Device_t;


Device_t Dev;


RollPitchYaw_t RPY_dev;

float Rad_to_Deg = 57.295f;


float GRAVITY_EARTH = 9.81;
uint8_t DataReadyBMIGyro = 0;
uint8_t DataReadyBMIAcc = 0;
uint8_t DataReadySPL = 0;
uint8_t DataReadyLPS25 = 0;

volatile float AverageTemp = 0.0f;

uint32_t time_tick = 0, Sample_time = 25;

volatile uint8_t ALTIcounter = 0;
volatile uint8_t MAGcounter = 0;
uint8_t ALTI_ON = 0;
uint8_t MAG_ON = 0;


// Filter coefficients
//float ALPHA_ACC
//float ALPHA_GYRO	0.8f
//float ALPHA_MAG 	0.5f

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int __io_putchar(int ch)
{
  if (ch == '\n') {
    __io_putchar('\r');
  }

  HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, HAL_MAX_DELAY);

  return 1;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{


	if(GPIO_Pin == BMI_GYRO_INT_Pin){
	//	 DataReadyBMIGyro = 1;
		}
	if(GPIO_Pin == BMI_ACC_INT_Pin){
	//DataReadyBMIAcc = 1;
			}
	if(GPIO_Pin == SPL_INT_Pin){
		//DataReadySPL = 1;
	}
	if(GPIO_Pin == LPS25_INT_Pin){
		//DataReadyLPS25 = 1;

	}

	if(GPIO_Pin == USER_BUTT_Pin){

		ALTIcounter++;
		MAGcounter++;

		if(ALTIcounter >= ALTIMETER_ON){
			HAL_TIM_OC_Stop_IT(&htim4, TIM_CHANNEL_1);

			MAGcounter = 0;
			ALTIcounter = 0;
			ALTI_ON ^= 1;

			HAL_GPIO_TogglePin(Sensors_Led_GPIO_Port,  Sensors_Led_Pin);



		}
		else if(ALTIcounter ==  ALTIMETER_OFF){
			HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_1);
		}

	}



}


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim4) {
    switch (HAL_TIM_GetActiveChannel(&htim4)) {
    case HAL_TIM_ACTIVE_CHANNEL_1:

    	ALTIcounter = 0;

    	if(MAGcounter == MAGNETOMETER_ON){
    		MAG_ON ^= 1;
    		HAL_GPIO_TogglePin(Status_Led_GPIO_Port,  Status_Led_Pin);
    		MAGcounter = 0;

    	}
    	HAL_TIM_OC_Stop_IT(&htim4, TIM_CHANNEL_1);

      break;

    default:
      break;
    }
  }
  if (htim == &htim3) {
    switch (HAL_TIM_GetActiveChannel(&htim3)) {
    case HAL_TIM_ACTIVE_CHANNEL_1:
    	//Some troubles on I2C bus sensor led blinking with 1Hz
      HAL_GPIO_TogglePin(Sensors_Led_GPIO_Port,  Sensors_Led_Pin);
      break;
    case HAL_TIM_ACTIVE_CHANNEL_2:
    	//Some critical troubles on I2C bus sensor led blinking with 2Hz
      HAL_GPIO_TogglePin(Sensors_Led_GPIO_Port,  Sensors_Led_Pin);
      break;
    case HAL_TIM_ACTIVE_CHANNEL_3:
    	// No I2C devices status blinking with 4Hz
        HAL_GPIO_TogglePin(Status_Led_GPIO_Port,  Status_Led_Pin);
        break;
    default:
      break;
    }
  }
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_I2C2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
 // HAL_I2C_EnableListen_IT(&hi2c1);
ScanI2CBus(&hi2c1); ScanI2CBus(&hi2c2);
CheckI2Cfaults(SPL06_INIT(&Dev.SPL, &hi2c1, SPL06_I2C_ADDRESS_I));
CheckI2Cfaults(BMI088_INIT(&Dev.BMI, &hi2c2));
CheckI2Cfaults(QMC5883L_INIT(&Dev.QMC,&hi2c1 , QMC5883L_ADDRESS));


// dummy read
QMC5883L_ReadMag(&Dev.QMC, 0.5f);
SPL06_READ_TEMP_PRESS(&Dev.SPL);
BMI088_ReadAcc(&Dev.BMI); BMI088_ReadGyro(&Dev.BMI);

INIT_ALTIMETER(&Dev.Altimeter, Dev.SPL.pressure_hPa);
DataPrepareForAlitmeter(&Dev.Altimeter,&Dev.SPL, 50);

InitRollPitchYawEstimationFunc(&RPY_dev);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
{


	  if(HAL_GetTick() - time_tick >= Sample_time)
	 	  {

		  time_tick = HAL_GetTick();

		  BMI088_ReadAcc(&Dev.BMI);
		  BMI088_ReadGyro(&Dev.BMI);
		  QMC5883L_ReadMag(&Dev.QMC, 0.6f);
		  SPL06_READ_TEMP_PRESS(&Dev.SPL);
		  Estimate_Altitude(&Dev.Altimeter ,&Dev.SPL,Dev.SPL.pressure_hPa, Dev.SPL.temp_cel,1);
		  EstimateRollPitchYawComplementaryFilter(&RPY_dev, &Dev.BMI,&Dev.QMC,0.1f,0.8f,Sample_time , 0.1f, ALTI_ON);

		  printf("%.3f,%.3f,%.3f,%.3f,%d,%d\n",RPY_dev.RollAngle , RPY_dev.PitchAngle,RPY_dev.YawAngle,Dev.Altimeter.Altitude_1, ALTI_ON, MAG_ON);



	  }






    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
