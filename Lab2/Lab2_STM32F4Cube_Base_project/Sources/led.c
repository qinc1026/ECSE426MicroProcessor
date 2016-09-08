/**
  ******************************************************************************
  * File Name          : led.c
  * Description        : A program which configures LED and overheating alarm
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
extern __IO int LED_Delay;
extern __IO int counter;

/* Private variables ---------------------------------------------------------*/
int flag, flag0;

/* Private function prototypes -----------------------------------------------*/

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
   * @brief	Turns on the alarm when the temeprature is above 60, and turn off when temperature goes down
   * @param temeprature: current temperature value
   * @retval None
   */
void Overheating_Alarm(float temperature){
	if (temperature > (float)MAX_TEMP){
		if(flag0 == 0){
			counter = 0;
			flag0 = 1;
		}
		if (counter >0){
			if(flag == 0){
				flag = 1;
				LED_Delay = 0;
			}
			else{
				if(LED_Delay  < 50000){
					/* turn on the first led */
					LED_Display(LED_BLUE, LED_GREEN);
				}
				else if(LED_Delay >= 50000 &&  LED_Delay < 2*50000){
					/* turn off the first led and turn on the second led */
					LED_Display(LED_GREEN, LED_ORANGE);
				}
				else if( LED_Delay >= 2*50000 &&   LED_Delay < 3*50000){
					/* turn off the second led and turn on the third led */
					LED_Display(LED_ORANGE, LED_RED);
				}
				else if(LED_Delay >= 3*50000 && LED_Delay < 4*50000){
					/* turn off the third led and turn on the fourth led */
					LED_Display(LED_RED, LED_BLUE);
				}
				else{
					LED_Display(LED_GREEN | LED_ORANGE | LED_RED | LED_BLUE, NULL);
					LED_Delay = 0;
				}
			}
		}
	}
	else{
		flag = 0;
		flag0 = 0;
		/* turn off all the leds */
		LED_Display(LED_GREEN | LED_ORANGE | LED_RED | LED_BLUE, NULL);
	}
}
