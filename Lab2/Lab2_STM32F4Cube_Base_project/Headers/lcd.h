/**
  ******************************************************************************
  * @file    lcd.h
  * @author  ECSE-426 Group 18
  * @version V1.0.0
  * @date    17-February-2015
  * @brief   Header file containing LCD setup and its basic functions
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H
#define __LCD_H

/* Exported functions --------------------------------------------------------*/

/* Configuration ***********************************/
void LCD_Config(void);

/* Initialization ***********************************/
void Function_Set(void);
void Display_and_Cursor(void);
void Set_Entry_Mode(void);
void Set_Display_Address(void);

/* Display operation functions ******************************************************/
void One_LCD_Display(int number);
void LCD_Display(float temperature);
void Clear_LCD_Display(void);

#endif /*__LCD_H */
