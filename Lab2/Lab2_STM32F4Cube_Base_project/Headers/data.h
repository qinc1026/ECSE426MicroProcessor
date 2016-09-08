/**
  ******************************************************************************
  * @file    data.h
  * @author  ECSE-426 Group 18
  * @version V1.0.0
  * @date    16-February-2015
  * @brief   Header file containing pin configurations and kalman state structure
  ******************************************************************************
  */

#ifndef _data_H
#define _data_H

/** @defgroup initial_kstate_define initial kalman state values define
  * @{
  */
#define DEF_q 0.070000
#define DEF_r 3.000000
#define DEF_x 0.000000
#define DEF_p 0.100000
#define DEF_k 0.000000

/** @defgroup Segment_pins_define	7-segment pins define
  * @brief GPIO Port E pins
  * @{
  */
#define	A		GPIO_PIN_7
#define	B		GPIO_PIN_8
#define	C		GPIO_PIN_9
#define D		GPIO_PIN_10
#define E		GPIO_PIN_11
#define F		GPIO_PIN_12
#define G		GPIO_PIN_13
#define DP		GPIO_PIN_14

/** @defgroup Digit_pins_define	Digits for 7-segment display pins define
  * @brief GPIO Port C pins
  * @{
  */
// 3 digits with degree symbol for 7-segment display
#define TENS		GPIO_PIN_0		//PortC
#define UNITS		GPIO_PIN_1		//PortC
#define DECIMAL		GPIO_PIN_2		//PortC
#define	DEGREE		GPIO_PIN_3		//PortC

/** @defgroup LCD_pins_define	LCD pins define
  * @brief GPIO Port C pins
  * @{
  */
#define	RS			GPIO_PIN_4
#define	RorW		GPIO_PIN_5
#define	Enable		GPIO_PIN_6
#define	Data0		GPIO_PIN_7
#define	Data1		GPIO_PIN_8
#define	Data2		GPIO_PIN_9
#define	Data3		GPIO_PIN_10
#define	Data4		GPIO_PIN_11
#define	Data5		GPIO_PIN_12
#define	Data6		GPIO_PIN_13
#define	Data7		GPIO_PIN_14

/** @defgroup LED_pins_define	LED pins define
  * @brief GIPO Port D pins
  * @{
  */
#define LED_GREEN 		GPIO_PIN_12
#define LED_ORANGE 		GPIO_PIN_13
#define LED_RED			GPIO_PIN_14
#define LED_BLUE		GPIO_PIN_15

/** @defgroup max_temp_define	overheating maximum temperature define
  * @{
  */
#define MAX_TEMP		25.0

/** 
  * @brief Kalman state initial structure definition  
  */ 
struct kalman_state
{
	float q;	/*!< process noise covariance */
	float r;	/*!< measurement noise covariance */
	float x;	/*!< value */
	float p;	/*!< estimation error covariance */
	float k;	/*!< kalman gain */
};

#endif	/* __DATA_H */
