/*******************************************************************************
  * @file    Thread_Segment.c
  * @author  Chaun Qin, Wei Wang
  * @version V1.0.0
  * @date    17-March-2016
  * @brief   This file initializes 7-segment as an output display,
  *		 	 			implements the Display thread which display temeprature values or rotated position,
  *						and function which creates and starts the thread	
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "data.h"
#include "tim.h"

void Thread_Segment (void const *argument);                 // thread function
osThreadId tid_Thread_Segment;                              // thread id
osThreadDef(Thread_Segment, osPriorityBelowNormal, 1, 0);

/* Extern variables	----------------------------------------------------------*/
extern int keypadresult;
extern float ADCdisplay;
extern float Rollangle;
extern float Pitchangle;
extern int flagTim3;

/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitC;
GPIO_InitTypeDef GPIO_InitE;
int first;
int second;
int third;
int flashflag;
int displayangleflag;

/**
   * @brief Numerical digit Conventions (0 - 9) for 7-segment display
   * @param  number: one-digit number to display
   * @retval None
   */
void One_Segment_Display(int number){
	switch(number){
		default:	/* Reset all pins */
			HAL_GPIO_WritePin(GPIOE, A, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, B, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, C, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, D, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, F, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, D, GPIO_PIN_RESET);

		case 0:		/* SET: ABCDEF */
			HAL_GPIO_WritePin(GPIOE, A, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, B, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, C, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, D, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, F, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, G, GPIO_PIN_RESET);
			break;
		case 1:		/* SET: BC */
			HAL_GPIO_WritePin(GPIOE, A, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, B, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, C, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, D, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, F, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, G, GPIO_PIN_RESET);
			break;
		case 2:		/* SET: ABDEG */
			HAL_GPIO_WritePin(GPIOE, A, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, B, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, C, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, D, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, F, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, G, GPIO_PIN_SET);
			break;
		case 3:		/* SET: ABCDG */
			HAL_GPIO_WritePin(GPIOE, A, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, B, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, C, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, D, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, F, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, G, GPIO_PIN_SET);
			break;
		case 4:		/* SET: BCFG */
			HAL_GPIO_WritePin(GPIOE, A, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, B, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, C, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, D, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, F, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, G, GPIO_PIN_SET);
			break;
		case 5:		/* SET: ACDFG */
			HAL_GPIO_WritePin(GPIOE, A, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, B, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, C, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, D, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, F, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, G, GPIO_PIN_SET);
			break;
		case 6:		/* SET: ACDEFG */
			HAL_GPIO_WritePin(GPIOE, A, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, B, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, C, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, D, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, F, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, G, GPIO_PIN_SET);
			break;
		case 7:		/* SET: ABC */
			HAL_GPIO_WritePin(GPIOE, A, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, B, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, C, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, D, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, F, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, G, GPIO_PIN_RESET);
			break;
		case 8:		/* SET:	ABCDEFG */
			HAL_GPIO_WritePin(GPIOE, A, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, B, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, C, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, D, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, F, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, G, GPIO_PIN_SET);
			break;
		case 9:		/* SET: ABCDFG */
			HAL_GPIO_WritePin(GPIOE, A, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, B, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, C, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, D, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, F, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, G, GPIO_PIN_SET);
			break;
		}
}

/**
   * @brief Display the first 3 digit of the angle (hundreds, tens, ones)
   * @param  degree: angle of the board's position
   * @retval None
   */
void Segments_DisplayXXX(float degree){
		first = degree/100;
		second = (degree-first*100)/10;
		third = (int)degree % 10;
		//first digit turned on
		HAL_GPIO_WritePin(GPIOC, digit1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, digit2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit4, GPIO_PIN_RESET);
		One_Segment_Display(first);
		TIM3_Delay(DISPLAY);
		
		//second digit turned on
		HAL_GPIO_WritePin(GPIOC, digit1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit4, GPIO_PIN_RESET);
		One_Segment_Display(second);
		TIM3_Delay(DISPLAY);
		
		//third digit turned on
		HAL_GPIO_WritePin(GPIOC, digit1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, digit4, GPIO_PIN_RESET);
		One_Segment_Display(third);
		TIM3_Delay(DISPLAY);
		
		//last digit turned on
		HAL_GPIO_WritePin(GPIOC, digit1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit4, GPIO_PIN_SET);
		/* SET: ABFG */
		HAL_GPIO_WritePin(GPIOE, A, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, B, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, C, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, D, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, F, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, G, GPIO_PIN_SET);
		TIM3_Delay(DISPLAY);
}

/**
   * @brief Display the first 3 digit of the angle (tens, ones, tenths)
   * @param  degree: angle of the board's position
   * @retval None
   */
void Segments_DisplayXXY(float degree){
		degree *=10;
		first = degree/100;
		second = (degree-first*100)/10;
		third = (int)degree % 10;
		//first digit turned on
		HAL_GPIO_WritePin(GPIOC, digit1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, digit2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit4, GPIO_PIN_RESET);
		One_Segment_Display(first);
		TIM3_Delay(DISPLAY);
	
		//second digit turned on
		HAL_GPIO_WritePin(GPIOC, digit1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, digit3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit4, GPIO_PIN_RESET);
		One_Segment_Display(second);
		TIM3_Delay(DISPLAY);
		
		//third digit turned on
		HAL_GPIO_WritePin(GPIOC, digit1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, digit4, GPIO_PIN_RESET);
		One_Segment_Display(third);
		TIM3_Delay(DISPLAY);
		
		//last digit turned on
		HAL_GPIO_WritePin(GPIOC, digit1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit4, GPIO_PIN_SET);
		/* SET: ABFG */
		HAL_GPIO_WritePin(GPIOE, A, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, B, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, C, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, D, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, F, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, G, GPIO_PIN_SET);
		TIM3_Delay(DISPLAY);
		
}

/**
   * @brief Display the first 3 digit of the angle (ones, tenths, hundredths)
   * @param  degree: angle of the board's position
   * @retval None
   */
void Segments_DisplayXYY(float degree){
		degree *=100;
		first = degree/100;
		second = (degree-first*100)/10;
		third = (int)degree % 10;
		//first digit turned on
		HAL_GPIO_WritePin(GPIOC, digit1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, digit2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, digit3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit4, GPIO_PIN_RESET);
		One_Segment_Display(first);
		TIM3_Delay(DISPLAY);
		
		//second digit turned on
		HAL_GPIO_WritePin(GPIOC, digit1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit4, GPIO_PIN_RESET);
		One_Segment_Display(second);
		TIM3_Delay(DISPLAY);
		
		//third digit turned on
		HAL_GPIO_WritePin(GPIOC, digit1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, digit4, GPIO_PIN_RESET);
		One_Segment_Display(third);
		TIM3_Delay(DISPLAY);
		
		//last digit turned on
		HAL_GPIO_WritePin(GPIOC, digit1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, digit4, GPIO_PIN_SET);
		/* SET: ABFG */
		HAL_GPIO_WritePin(GPIOE, A, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, B, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, C, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, D, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, F, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, G, GPIO_PIN_SET);
		TIM3_Delay(DISPLAY);
		
}

/**
   * @brief 7-segment display performs the value of borad's position angles in degree
   * @param degree: angle of the board's position
   * @retval None
   */
void Segments_Display(float degree){
	if(degree >= 100){
		Segments_DisplayXXX(degree);
	}
	else if (degree < 100 && degree >= 10){
		Segments_DisplayXXY(degree);
	}
	else{
		Segments_DisplayXYY(degree);
	}
}

/**
   * @brief Turns off the 7-segment display
   * @retval None
   */
void Segments_DisplayNull(){
	HAL_GPIO_WritePin(GPIOC, digit1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, digit2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, digit3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, digit4, GPIO_PIN_RESET);
	TIM3_Delay(4 * DISPLAY);
}

/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_Segment (void) {

  tid_Thread_Segment = osThreadCreate(osThread(Thread_Segment ), NULL); // Start LED_Thread
  if (!tid_Thread_Segment) return(-1); 
  return(0);
}

/*----------------------------------------------------------------------------
 *      Thread  'Segment_Thread': Displays temeprature or position of the board
 *---------------------------------------------------------------------------*/
	void Thread_Segment (void const *argument) {
		while(1){
			osSignalWait(0x04, osWaitForever);
			if (ADCdisplay < MAX_TEMP){
				flashflag = 0;
			}
			else{
				flashflag++;
			}
			if (flashflag == 6){
				flashflag = 0;
			}
			if (keypadresult == 10 && flashflag < 3){
				Segments_Display(ADCdisplay);
				displayangleflag = 0;
			}
			else if (keypadresult == 11){
				displayangleflag = 1;
				//Segments_Display(Rollangle);
			}
			else if (keypadresult == 1 && displayangleflag == 1  && flashflag < 3){
				Segments_Display(Rollangle);
			}
			else if (keypadresult == 2 && displayangleflag == 1  && flashflag < 3){
				Segments_Display(Pitchangle);
			}
			else if (flashflag >= 3){
				Segments_DisplayNull();
			}
			}
	}

/*----------------------------------------------------------------------------
 *      Initialize the GPIO associated with the four digits 7-ssegment display
 *---------------------------------------------------------------------------*/
	void initializeSegment_IO (void){

		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOE_CLK_ENABLE();
	
		GPIO_InitC.Pin = digit1 | digit2 | digit3 | digit4;
		GPIO_InitC.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitC.Pull = GPIO_PULLUP;
		GPIO_InitC.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOC, &GPIO_InitC);
		
		GPIO_InitE.Pin = A | B | C | D | E | F | G | DP;
		GPIO_InitE.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitE.Pull = GPIO_PULLUP;
		GPIO_InitE.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOE, &GPIO_InitE);
	}
	
/*----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
