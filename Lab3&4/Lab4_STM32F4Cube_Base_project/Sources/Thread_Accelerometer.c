/*******************************************************************************
  * @file    Thread_Accelerometer.c
  * @author  Chuan Qin, Wei Wang
  * @version V1.0.0
  * @date    17-March-2016
  * @brief   This file initializes accelerometer to read position,
	*						implements the accelerometer thread which calculates the angles,
  *						and function which creates and starts the thread	
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "data.h"
#include "math.h"
#include "lis3dsh.h"
#include "filter.h"

void Thread_Accelerometer (void const *argument);                 // thread function
osThreadId tid_Thread_Accelerometer;                              // thread id
osThreadDef(Thread_Accelerometer, osPriorityNormal, 1, 0);

/* Extern variables	----------------------------------------------------------*/
extern osThreadId tid_Thread_ADC;

/* Private variables ---------------------------------------------------------*/
LIS3DSH_InitTypeDef LIS3DSH_Ini;
LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_InterruptConfig;
SPI_HandleTypeDef SPI_Handle;
float output[3];
float accx;
float accy;
float accz;
float Rollangle;
float Pitchangle;
int setKstate = 0;
struct kalman_state stateX;
struct kalman_state stateY;
struct kalman_state stateZ;

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
   * @brief Calcultates the roll angle of the board around front-to-back axis
   * @param  out[]: the angle output from accelerometer
   * @retval None
   */
void GetRollangle(volatile float out[]){
	Rollangle = (float)atan(out[0]/sqrt(out[1]*out[1]+out[2]*out[2]));
	Rollangle = Rollangle *180/2/asin(1) + 90;
}

/**
   * @brief Calcultates the pitch angle of the board around side-to-side axis
   * @param  out[]: the angle output from accelerometer
   * @retval None
   */
void GetPitchangle(volatile float out[]){
	Pitchangle = (float)atan(out[1]/sqrt(out[0]*out[0]+out[2]*out[2]));
	Pitchangle = Pitchangle *180/2/asin(1) + 90;
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

/**
  * @brief  This function handles EXTIO Handler exception.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_Accelerometer (void) {

  tid_Thread_Accelerometer = osThreadCreate(osThread(Thread_Accelerometer), NULL); // Start LED_Thread
  if (!tid_Thread_Accelerometer) return(-1); 
  return(0);
}

/*----------------------------------------------------------------------------
 *      Thread  'Accelerometer_Thread': Updates roll and pitch angles of the board
 *---------------------------------------------------------------------------*/
	void Thread_Accelerometer (void const *argument) {
		while(1){
			osSignalWait(0x02, osWaitForever);
			GetPitchangle(output);
			GetRollangle(output);
			
			osSignalSet(tid_Thread_ADC, 0x01);
			osDelay(SLEEP);
			//TIM3_Delay(SLEEP);
			}
	}
/*----------------------------------------------------------------------------
 *      Initialize the SPI associated with the accelerometer
 *---------------------------------------------------------------------------*/
	void initializeAccelerometer_IO (void){
		LIS3DSH_Ini.Axes_Enable						= LIS3DSH_XYZ_ENABLE;
		LIS3DSH_Ini.Continous_Update 				= LIS3DSH_ContinousUpdate_Enabled;
		LIS3DSH_Ini.Full_Scale 						= LIS3DSH_FULLSCALE_2;
		LIS3DSH_Ini.Power_Mode_Output_DataRate 		= LIS3DSH_DATARATE_25;
		LIS3DSH_Init(&LIS3DSH_Ini);
	
		LIS3DSH_InterruptConfig.Dataready_Interrupt  = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
		LIS3DSH_InterruptConfig.Interrupt_signal	 = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
		LIS3DSH_InterruptConfig.Interrupt_type 		 = LIS3DSH_INTERRUPT_REQUEST_PULSED;
		LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_InterruptConfig);
		HAL_SPI_MspInit(&SPI_Handle);
	
		HAL_NVIC_EnableIRQ(EXTI0_IRQn);
		HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	}

/*----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
