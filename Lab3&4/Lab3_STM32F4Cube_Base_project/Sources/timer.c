/**
  ******************************************************************************
  * File Name          : timer.c
  * Description        : set and config timer
  * Author			   		 : Chuan Qin, Wei Wang
  * Version            : 1.0.0
  * Date			 			   : March 5th, 2016
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "data.h"
#include "led.h"

/* extern variables	----------------------------------------------------------*/
extern int flagTim;

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef TIM_Handle;
int ledflag;
int count;


/**
   * @brief Setup hardware timer
   * @retval None
   */
void Timer_Config(void){
	__TIM3_CLK_ENABLE();
	
	TIM_Handle.Init.Prescaler = 83 ;
	TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM_Handle.Init.Period = 99;
	TIM_Handle.Instance = TIM3;
	HAL_TIM_Base_Init(&TIM_Handle);     // Init timer
	HAL_TIM_Base_Start_IT(&TIM_Handle); // start timer interrupts
	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

/**
   * @brief Set hardware timer falg when interrupt happened
   * @retval None
   */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
 flagTim = 1;
}

/**
   * @brief Provides delay time in 100 microseconds based on variable incremented
   * @param counter: specifies the delay time length, in 100 microseconds.
   * @retval None
   */
void TIM3_Delay(int counter){
	count = 0;
	flagTim = 0;
	while(count < counter){
		if (flagTim == 1){
			flagTim = 0;
			count++;
		}
	}
}
