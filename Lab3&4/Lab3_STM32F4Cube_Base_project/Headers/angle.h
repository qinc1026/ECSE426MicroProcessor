/**
  ******************************************************************************
  * @file    angle.h
  * @author  ECSE-426 Group 18
  * @version V1.0.0
  * @date    07-March-2015
  * @brief   Header file containing accelerometer setup and its basic functions
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ANGLE_H
#define __ANGLE_H

/* Exported functions --------------------------------------------------------*/
 
/* Configuration ***********************************/
void Accelerometer_Config(void);

/* Display operation functions ******************************************************/
void Calibration(volatile float out[]);
float Getangle(volatile float out[]);

#endif /*__ANGLE_H */
