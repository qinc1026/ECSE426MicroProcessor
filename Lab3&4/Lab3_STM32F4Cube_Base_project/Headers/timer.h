/**
  ******************************************************************************
  * @file    timer.h
  * @author  ECSE-426 Group 18
  * @version V1.0.0
  * @date    07-March-2015
  * @brief   Header file containing hardware timer setup and its basic functions
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER_H
#define __TIMER_H

/* Exported functions --------------------------------------------------------*/
 
/* Configuration ***********************************/
void Timer_Config(void);

/* Display operation functions ******************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void TIM3_Delay(int counter);

#endif /*__LED_H */
