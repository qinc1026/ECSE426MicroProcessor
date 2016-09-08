/**
  ******************************************************************************
  * File Name          : keypad.c
  * Description        : A program which configures 4*4 matrix keypad
  * Author			   		 : Chuan Qin, Wei Wang
  * Version            : 1.0.0
  * Date			   			 : March 3rd, 2016
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "data.h"

/* Private variables ---------------------------------------------------------*/
GPIO_PinState PinCol1;
GPIO_PinState PinCol2;
GPIO_PinState PinCol3;
GPIO_PinState PinCol4;
GPIO_PinState PinRow1;
GPIO_PinState PinRow2;
GPIO_PinState PinRow3;
GPIO_PinState PinRow4;
uint8_t temp;
uint8_t data;


/**
   * @brief Configures GPIO for keypad
   * @retval None
   */
void Keypad_Config(void){
	GPIO_InitTypeDef GPIO_InitD;
	
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitD.Pin = Col1 | Col2 | Col3 | Col4 | Row1 | Row2 | Row3 | Row4;
	GPIO_InitD.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitD.Pull = GPIO_PULLUP;
	GPIO_InitD.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitD);
}

/**
   * @brief Setup each column of the keypad
   * @retval None
   */
void Keypad_Columninput(void){
	GPIO_InitTypeDef GPIO_InitD;
	
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitD.Pin =  Col1 | Col2 | Col3 | Col4;
	GPIO_InitD.Mode = GPIO_MODE_INPUT;
	GPIO_InitD.Pull = GPIO_PULLUP;
	GPIO_InitD.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitD);
	
	GPIO_InitD.Pin = Row1 | Row2 | Row3 | Row4;
	GPIO_InitD.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitD.Pull = GPIO_PULLUP;
	GPIO_InitD.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitD);
	
	HAL_GPIO_WritePin(GPIOD, Row1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, Row2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, Row3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, Row4, GPIO_PIN_RESET);
}

/**
   * @brief Setup each row of the keypad
   * @retval None
   */
void Keypad_Rowinput(void){
	GPIO_InitTypeDef GPIO_InitD;
	
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitD.Pin =  Col1 | Col2 | Col3 | Col4;
	GPIO_InitD.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitD.Pull = GPIO_PULLUP;
	GPIO_InitD.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitD);
	
	GPIO_InitD.Pin = Row1 | Row2 | Row3 | Row4;
	GPIO_InitD.Mode = GPIO_MODE_INPUT;
	GPIO_InitD.Pull = GPIO_PULLUP;
	GPIO_InitD.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitD);
	
	HAL_GPIO_WritePin(GPIOD, Col1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, Col2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, Col3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, Col4, GPIO_PIN_RESET);
}

/**
   * @brief Read the number from the keypad
   * @retval the number entered by using keypad
   */
int Keypad_Read(void){
	Keypad_Columninput();
	PinCol1=HAL_GPIO_ReadPin(GPIOD,Col1);
	PinCol2=HAL_GPIO_ReadPin(GPIOD,Col2);
	PinCol3=HAL_GPIO_ReadPin(GPIOD,Col3);
	PinCol4=HAL_GPIO_ReadPin(GPIOD,Col4);	
	temp = (((PinCol4 == GPIO_PIN_SET)<<3) +((PinCol3 == GPIO_PIN_SET)<<2) + ((PinCol2 == GPIO_PIN_SET)<<1) + ((PinCol1 == GPIO_PIN_SET)));
	data = temp<<4;
	
	Keypad_Rowinput();
	PinRow1=HAL_GPIO_ReadPin(GPIOD,Row1);
	PinRow2=HAL_GPIO_ReadPin(GPIOD,Row2);
	PinRow3=HAL_GPIO_ReadPin(GPIOD,Row3);
	PinRow4=HAL_GPIO_ReadPin(GPIOD,Row4);
	temp = (((PinRow4 == GPIO_PIN_SET)*8) +((PinRow3 == GPIO_PIN_SET)*4) + ((PinRow2 == GPIO_PIN_SET)*2) + ((PinRow1 == GPIO_PIN_SET)));
	data |= temp;
	
	if(data != 0xFF){
		switch(data){
			default:
				return -2;
			
			case 0xEE:
				return 1;
			case 0xED:
				return 4;
			case 0xEB:
				return 7;
			case 0xDE:
				return 2;
			case 0xDD:
				return 5;
			case 0xDB:
				return 8;
			case 0xD7:
				return 0;
			case 0xBE:
				return 3;
			case 0xBD:
				return 6;
			case 0xBB:
				return 9;
			//clear (C)
			case 0x7B:
				return 12;
			//enter (*)
			case 0xE7:
				return 11;
		}
	}
	else{
		return -1;
	}
	
}
