/**
  ******************************************************************************
  * File Name          : segment.c
  * Description        : A program which configures 7-segments display
  * Author			   : Chuan Qin, Wei Wang
  * Version            : 1.0.0
  * Date			   : February 14th, 2016
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "data.h"

/* extern variables	----------------------------------------------------------*/
extern __IO int Segments_Delay;

/* Private variables ---------------------------------------------------------*/
int tens, units, decimal;

/* Private function prototypes -----------------------------------------------*/

/**
   * @brief Configures GPIO for 7-segment display
   * @retval None
   */
void Display_Config(void){
	GPIO_InitTypeDef GPIO_InitC;
	GPIO_InitTypeDef GPIO_InitE;
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	GPIO_InitC.Pin = TENS | UNITS | DECIMAL | DEGREE |RS | RorW | Enable| Data0 | Data1 | Data2 | Data3 | Data4 | Data5 | Data6 | Data7 ;
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
   * @brief Four digits of 7-segment display with decimal point
			 This functions have 4 specific cases:
			 T for Tens digit, U for Units digit, D for Tenths digit, O for degree
   * @param  digit: A character to display
   * @retval None
   */
void Digit_Segments_Display(char digit){
	switch(digit){
		default:
			HAL_GPIO_WritePin(GPIOC, TENS, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, UNITS, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, DECIMAL, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, DEGREE, GPIO_PIN_RESET);

		case 'T':
			HAL_GPIO_WritePin(GPIOC, TENS, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, UNITS, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, DECIMAL, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, DEGREE, GPIO_PIN_RESET);
			break;
		case 'U':	/* both units digit and decimal point are on */
			HAL_GPIO_WritePin(GPIOC, TENS, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, UNITS, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, DECIMAL, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, DEGREE, GPIO_PIN_RESET);
			break;
		case 'D':
			HAL_GPIO_WritePin(GPIOC, TENS, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, UNITS, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, DECIMAL, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, DEGREE, GPIO_PIN_RESET);
			break;
		case 'O':	/* set degree symbol as rightmost digit */
			HAL_GPIO_WritePin(GPIOC, TENS, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, UNITS, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, DP, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, DECIMAL, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, DEGREE, GPIO_PIN_SET);
			/* SET: ABFG */
			HAL_GPIO_WritePin(GPIOE, A, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, B, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, C, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, D, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, E, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, F, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, G, GPIO_PIN_SET);
			break;
	}
}

/**
   * @brief Extracts 3 individual digits of a temperature value and displays
   * @param  temperature
   * @retval None
   */
void Segments_Display(float temperature){
	tens =  temperature / 10;
	units =  (int)temperature % 10;
	decimal = (int)(temperature * 10) % 10;

	if (tens != 0){

		Digit_Segments_Display('T');
		One_Segment_Display(tens);
		Segments_Delay = 0;
		while(Segments_Delay <1900){
		}
	}
	Digit_Segments_Display('U');
	One_Segment_Display(units);
	Segments_Delay = 0;
	while(Segments_Delay <1900){
	}
	Digit_Segments_Display('D');
	One_Segment_Display(decimal);
	Segments_Delay = 0;
	while(Segments_Delay <1900){
	}
	Digit_Segments_Display('O');
	Segments_Delay = 0;
	while(Segments_Delay <1900){
	}
}
