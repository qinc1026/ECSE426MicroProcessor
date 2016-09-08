/**
  ******************************************************************************
  * File Name          : angle.c
  * Description        : A program which calculates angle
  * Author			   		 : Chuan Qin, Wei Wang
  * Version            : 1.0.0
  * Date			   			 : March 3rd, 2016
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "data.h"
#include "lis3dsh.h"
#include <math.h>
#include "filter.h"

/* extern variables	----------------------------------------------------------*/
extern __IO float output[3];

/* Private variables ---------------------------------------------------------*/
LIS3DSH_InitTypeDef LIS3DSH_Ini;
LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_InterruptConfig;
SPI_HandleTypeDef SPI_Handle;

float accx;
float accy;
float accz;
int setKstate = 0;
struct kalman_state stateX;
struct kalman_state stateY;
struct kalman_state stateZ;


/**
   * @brief Configures accelerometer
   * @retval None
   */
void Accelerometer_Config(void){
	LIS3DSH_Ini.Axes_Enable = LIS3DSH_XYZ_ENABLE;
	LIS3DSH_Ini.Continous_Update = LIS3DSH_ContinousUpdate_Enabled;
	LIS3DSH_Ini.Full_Scale = LIS3DSH_FULLSCALE_2;
	LIS3DSH_Ini.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;
	LIS3DSH_Init(&LIS3DSH_Ini);
	
	LIS3DSH_InterruptConfig.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
	LIS3DSH_InterruptConfig.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	LIS3DSH_InterruptConfig.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;
	LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_InterruptConfig);
	HAL_SPI_MspInit(&SPI_Handle);
	
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
}

/**
   * @brief Calibrates the angle
   * @param  out[]: the angle given by accelerometer
   * @retval None
   */
void Calibration(volatile float out[]){
	accx = out[0]/1000;
	accy = out[1]/1000;
	accz = out[2]/1000;
	out[0] = acc11*accx +acc12*accy +acc13*accz +acc10;
	out[1] = acc21*accx +acc22*accy +acc23*accz +acc20;
	out[2] = acc31*accx +acc32*accy +acc33*accz +acc30;
}

/**
   * @brief Calcultates the angle of the board
	 * @param  out[]: the angle output from accelerometer
   * @retval angle
   */
float Getangle(volatile float out[]){
	float angle = (float)atan(out[0]/sqrt(out[1]*out[1]+out[2]*out[2]));
	angle = angle *180/2/asin(1) + 90;
	return angle;
}

/**
   * @brief Calibrate the output from accelerometer when the interrupt happened
   * @param  GPIO_Pin: Pin for interrupt signal
   * @retval None
   */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_0){
		LIS3DSH_ReadACC(output);
		Calibration(output);
		
		if (setKstate == 0){
			Kstate_Initialization(&stateX);
			Kstate_Initialization(&stateY);
			Kstate_Initialization(&stateZ);
			Set_Kstate_Parameters(&stateX, 0.03, 0.048, output[0]);
			Set_Kstate_Parameters(&stateY, 0.03, 0.048, output[1]);
			Set_Kstate_Parameters(&stateZ, 0.07, 0.048, output[2]);
			setKstate = 1;
		}
		output[0] = Kalmanfilter_C(&stateX, output[0]);
		output[1] = Kalmanfilter_C(&stateY, output[1]);
		output[2] = Kalmanfilter_C(&stateZ, output[2]);
	}
}
