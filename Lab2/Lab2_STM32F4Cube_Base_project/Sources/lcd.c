/**
  ******************************************************************************
  * File Name          : lcd.c
  * Description        : A program which configures LCD and its display presentation
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
extern __IO int LCD_Delay;

/* Private variables ---------------------------------------------------------*/
int l_tens, l_units, l_decimal;

/* Private function prototypes -----------------------------------------------*/

/**
   * @brief Configures GPIO for LCD display
   * @retval None
   */
void LCD_Config(void){
	GPIO_InitTypeDef GPIO_InitC;

	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitC.Pin = RS | RorW | Enable| Data0 | Data1 | Data2 | Data3 | Data4 | Data5 | Data6 | Data7 ;
	GPIO_InitC.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitC.Pull = GPIO_PULLUP;
	GPIO_InitC.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitC);
}

/**
   * @brief Numerical digit presentation for LCD
   * @param number
   * @retval None
   */
void One_LCD_Display(int number){
	switch(number){
		default:	/* Reset all pins */
			HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);
		case 0:		/* SET (D7 - D0): 00110000 */
			HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);
			break;
		case 1:		/* SET (D7 - D0): 00110001 */
			HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);
			break;
		case 2:		/* SET (D7 - D0): 00110010 */
			HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);
			break;
		case 3:		/* SET (D7 - D0): 00110011 */
			HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);
			break;
		case 4:		/* SET (D7 - D0): 00110100 */
			HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);
			break;
		case 5:		/* SET (D7 - D0): 00110101 */
			HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);
			break;
		case 6:		/* SET (D7 - D0): 00110110 */
			HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);
			break;
		case 7:		/* SET (D7 - D0): 00110111 */
			HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);
			break;
		case 8:		/* SET (D7 - D0):	00111000 */
			HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);
			break;
		case 9:		/* SET (D7 - D0): 00111001 */
			HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);
			break;
		case 10:	/* Decimal Point (D7 - D0): 00101110 */
			HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);
			break;
		case 11:	/* Degree Symbol (D7 - D0): 11011111 */
			HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_SET);
			break;		
		case 12:	/* Celsius (D7 - D0): 01000011 */
			HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);
			break;		
		}
		HAL_GPIO_WritePin(GPIOC, RS, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, RorW, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, Enable, GPIO_PIN_SET);
		LCD_Delay = 0;
		while(LCD_Delay < 10){}
		HAL_GPIO_WritePin(GPIOC, Enable, GPIO_PIN_RESET);
		LCD_Delay = 0;
		while(LCD_Delay < 37){}
}

/**
   * @brief	Clears LCD display
   * @retval None
   */
void Clear_LCD_Display(void){
	/* SET (D7 - D0):00000001 */
	HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(GPIOC, RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, RorW, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Enable, GPIO_PIN_SET);
	LCD_Delay = 0;
	while(LCD_Delay < 10){}
	HAL_GPIO_WritePin(GPIOC, Enable, GPIO_PIN_RESET);
	LCD_Delay = 0;
	while(LCD_Delay < 1600){}
}

/**
   * @brief	Setup the display location on LCD
   * @retval None
   */
void Set_Display_Address(void){
	/* SET (D7 - D0): 10000101 */
	HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(GPIOC, RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, RorW, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Enable, GPIO_PIN_SET);
	LCD_Delay = 0;
	while(LCD_Delay < 10){}
	HAL_GPIO_WritePin(GPIOC, Enable, GPIO_PIN_RESET);
	LCD_Delay = 0;
	while(LCD_Delay < 37){}
}

/**
   * @brief	Setup LCD 8 bits 2 lines
   * @retval None
   */
void Function_Set(void){
	/* SET (D7 - D0): 00111000 */
	HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOC, RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, RorW, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Enable, GPIO_PIN_SET);
	LCD_Delay = 0;
	while(LCD_Delay < 10){}
	HAL_GPIO_WritePin(GPIOC, Enable, GPIO_PIN_RESET);
	LCD_Delay = 0;
	while(LCD_Delay < 37){}
}

/**
   * @brief	Turns on display and cursor
   * @retval None
   */
void Display_and_Cursor(void){
	/* SET (D7 - D0): 00001111 */
	HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(GPIOC, RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, RorW, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Enable, GPIO_PIN_SET);
	LCD_Delay = 0;
	while(LCD_Delay < 10){}
	HAL_GPIO_WritePin(GPIOC, Enable, GPIO_PIN_RESET);
	LCD_Delay = 0;
	while(LCD_Delay < 37){}
}

/**
   * @brief	Setup entry mode and increases cursor position without shifting display
   * @retval None
   */
void Set_Entry_Mode(void){
	/* SET (D7 - D0): 00000110 */
	HAL_GPIO_WritePin(GPIOC, Data0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, Data2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, Data3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Data7, GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(GPIOC, RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, RorW, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, Enable, GPIO_PIN_SET);
	LCD_Delay = 0;
	while(LCD_Delay < 10){}
	HAL_GPIO_WritePin(GPIOC, Enable, GPIO_PIN_RESET);
	LCD_Delay = 0;
	while(LCD_Delay < 37){}
}

/**
   * @brief Extracts 3 individual digits of given number and display on LCD
   * @param temperature
   * @retval None
   */
void LCD_Display(float temperature){
	l_tens =  temperature / 10;
	l_units =  (int)temperature % 10;
	l_decimal = (int)(temperature * 10) % 10;

	if (l_tens != 0){
		One_LCD_Display(l_tens);
	}
	One_LCD_Display(l_units);
	One_LCD_Display(10);
	One_LCD_Display(l_decimal);
	One_LCD_Display(11);
	One_LCD_Display(12);
}
