/**
  ******************************************************************************
  * File Name          : led.c
  * Description        : A program which configures LED and overheating alarm
  * Author			 		 	 : Chuan Qin, Wei Wang
  * Version            : 1.0.0
  * Date			   			 : Feburary 14th, 2016
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "data.h"

/* Private variables ---------------------------------------------------------*/
int flag, flag0;


/**
   * @brief Setup four LED lights (green, orange, red, blue)
   * @retval None
   */
void LED_Config(void){
	GPIO_InitTypeDef GPIO_InitD;

	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	GPIO_InitD.Pin = LED_GREEN | LED_ORANGE | LED_RED | LED_BLUE;
	GPIO_InitD.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitD.Pull = GPIO_NOPULL;
	GPIO_InitD.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitD);
}

/**
   * @brief	Turns on a specific LED light
   * @param	 turn_off: the LED light that needs to be off
   * @param	 turn_on: the LED light that needs to be on
   * @retval None
   */
void LED_Display(uint32_t turn_off, uint32_t turn_on){
	HAL_GPIO_WritePin(GPIOD, turn_off, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, turn_on, GPIO_PIN_SET);
}

/**
   * @brief	Turns on a specific LED light
   * @param	 keypad: the target angle entered by keypad
   * @param	 degree: the angle measured by accelerometer
   * @retval case number
   */
int Scenario_Signal(int keypad, float degree){
	int min = keypad -5;
	int max = keypad+5;
	
	if(degree > max){
		LED_Display(LED_ORANGE | LED_RED | LED_BLUE, LED_GREEN);
		return 1;
	}
	else if(degree < min){
		LED_Display(LED_ORANGE | LED_GREEN | LED_BLUE, LED_RED);
		return 2;
	}
	else{
		LED_Display( LED_GREEN | LED_RED, LED_BLUE | LED_ORANGE);
		return 3;
	}
}
