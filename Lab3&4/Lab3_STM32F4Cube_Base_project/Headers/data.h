#ifndef _data_H
#define _data_H

/** @defgroup initial_kstate_define initial kalman state values define
  * @{
  */
#define DEF_q 0.100000
#define DEF_r 0.100000
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
#define DP	GPIO_PIN_14

/** @defgroup 4*4 matrix keypad pins define
  * @brief GPIO Port D pins
  * @{
  */
#define	Col1		GPIO_PIN_10
#define	Col2		GPIO_PIN_1
#define	Col3		GPIO_PIN_2
#define Col4		GPIO_PIN_3
#define Row1		GPIO_PIN_9
#define Row2		GPIO_PIN_5
#define Row3		GPIO_PIN_6
#define Row4		GPIO_PIN_7

/** @defgroup Digit_pins_define	Digits for 7-segment display pins define
  * @brief GPIO Port C pins
  * @{
  */
#define digit1		GPIO_PIN_15
#define digit2		GPIO_PIN_1
#define digit3		GPIO_PIN_2
#define	digit4		GPIO_PIN_3

/** @defgroup LED_pins_define	LED pins define
  * @brief GIPO Port D pins
  * @{
  */
#define LED_GREEN 	GPIO_PIN_12
#define LED_ORANGE 	GPIO_PIN_13
#define LED_RED			GPIO_PIN_14
#define LED_BLUE		GPIO_PIN_15

/** @defgroup data of Position Required For Full Calibration
  * @{
  */
#define acc11		0.994138083545817f
#define acc21		-0.021832092768331f
#define acc31		-0.001001098568320f
#define acc12		0.018260880674360f
#define acc22		0.961011356119953f
#define acc32		-0.021178545356665f
#define acc13		0.012006312335592f
#define acc23		0.044758274939461f
#define acc33		1.006413535148700f
#define acc10		-0.004835453911860f
#define acc20		0.018264113153855f
#define acc30		-0.021482451164122f

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
