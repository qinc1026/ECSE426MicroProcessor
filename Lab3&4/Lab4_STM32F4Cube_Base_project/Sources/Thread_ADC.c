/*******************************************************************************
  * @file    Thread_ADC.c
  * @author  Chaun Qin, Wei Wang
  * @version V1.0.0
  * @date    17-March-2016
  * @brief   This file initializes ADC, implements the ADC thread 
  *			 			which read temperature values and converts to degree celsius,
	*					  and function which creates and starts the thread	
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "data.h"
#include "filter.h"
#include  "tim.h"

void Thread_ADC (void const *argument);                 // thread function
osThreadId tid_Thread_ADC;                              // thread id
osThreadDef(Thread_ADC, osPriorityNormal, 1, 0);

/* Extern variables	----------------------------------------------------------*/
extern int flagTim;
extern osThreadId tid_Thread_Segment;

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef ADC1_Handle;
uint32_t ADC_Value;
struct kalman_state stateADC;
float temprature;
int flag2 = 0;
int flag3 = 0;
int cycle = 0;
float ADCdisplay;
float sum;

/**
   * @brief Calculates the temperature using the formula as following:
			Temperature(in C) = {(Vsense - V25) / Avg_Slope} + 25
			Vsense (Vsense value at 25 degree Celsius) = 0.76 V
			Avg_Slope (average slope of the temperature vs. Vsense curve) = 2.5 mV
   * @param  voltage: the reading value obtained by ADC
   * @retval value in temperature format in Celsius
   */
float Convert_Voltage_To_Celsius(uint32_t voltage){
	float Temperature = (float)voltage;
	Temperature *= 3000;
	Temperature /= 0xfff;			/* Reading in mV */
	Temperature /= (float)1000.0;	/* Reading in Volts */
	Temperature -= (float)0.760;	/* Substract the reference voltage at 25 degree Celsius */
	Temperature /= (float)0.0025;	/* Divide by slope 2.5mV */
	Temperature += (float)25.0;		/* Add the 25 degree Celsius */
	
	return Temperature;
}

/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_ADC (void) {

  tid_Thread_ADC = osThreadCreate(osThread(Thread_ADC ), NULL); // Start LED_Thread
  if (!tid_Thread_ADC) return(-1); 
  return(0);
}

/*----------------------------------------------------------------------------
 *      Thread  'ADC_Thread': Retrieves and converts temperature readings
 *---------------------------------------------------------------------------*/
	void Thread_ADC (void const *argument) {
		
		HAL_ADC_Start(&ADC1_Handle);
		HAL_ADC_PollForConversion(&ADC1_Handle, 10);
		ADC_Value = HAL_ADC_GetValue(&ADC1_Handle);
		__HAL_ADC_CLEAR_FLAG(&ADC1_Handle, ADC_FLAG_EOC);
		temprature = Convert_Voltage_To_Celsius(ADC_Value);
		Set_Kstate_Parameters(&stateADC, 0.07, 3, temprature);
		while(1){
			osSignalWait(0x01, osWaitForever);
			if(flagTim > 100){
				flagTim = 0;
				HAL_ADC_Start(&ADC1_Handle);
				HAL_ADC_PollForConversion(&ADC1_Handle, 10);
				ADC_Value = HAL_ADC_GetValue(&ADC1_Handle);
				__HAL_ADC_CLEAR_FLAG(&ADC1_Handle, ADC_FLAG_EOC);
				temprature = Convert_Voltage_To_Celsius(ADC_Value);
				temprature = Kalmanfilter_C(&stateADC, temprature);
				if(flag3 == 0){
					ADCdisplay = temprature;
					flag3 = 1;
				}
				sum += temprature;
				flag2++;
				if(flag2 == 10){
					ADCdisplay = sum / 10;
					sum = 0;
					flag2 = 0;
				}
			}
			osSignalSet(tid_Thread_Segment, 0x04);
			osDelay(SLEEP);
		}	
	}
	
/*----------------------------------------------------------------------------
 *      Initialize the ADC 1 associated with the channel 16
 *---------------------------------------------------------------------------*/
	void initializeADC_IO (void){
	ADC_ChannelConfTypeDef ADC_Channel;
	ADC1_Handle.Instance = ADC1;
	
	/* Enable the GPIO clock for the used pin and the ADC clock */
	__HAL_RCC_ADC1_CLK_ENABLE();
	
	/* Initialize the configuration of ADC */
	ADC1_Handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV6;
	ADC1_Handle.Init.Resolution = ADC_RESOLUTION_12B;
	ADC1_Handle.Init.ScanConvMode = DISABLE;
	ADC1_Handle.Init.ContinuousConvMode = DISABLE;
	ADC1_Handle.Init.DiscontinuousConvMode = DISABLE;
	ADC1_Handle.Init.NbrOfDiscConversion = 0;
	ADC1_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADC1_Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	ADC1_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC1_Handle.Init.NbrOfConversion = 1;
	ADC1_Handle.Init.DMAContinuousRequests = DISABLE;
	ADC1_Handle.Init.EOCSelection = DISABLE;
	HAL_ADC_Init(&ADC1_Handle);
	
	/* Initialize the ADC Channel configuration */
	ADC_Channel.Channel = ADC_CHANNEL_16;
	ADC_Channel.Rank = 1;
	ADC_Channel.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	ADC_Channel.Offset = 0;
	
	/* Set up the ADC Channel */
	if (HAL_ADC_ConfigChannel(&ADC1_Handle, &ADC_Channel) != HAL_OK) {
		Error_Handler(ADC_CH_CONFIG_FAIL);
	}
}

/*----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
