/**
  ******************************************************************************
  * @file    keypad.h
  * @author  ECSE-426 Group 18
  * @version V1.0.0
  * @date    07-March-2015
  * @brief   Header file containing keypad setup and its basic functions
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEYPAD_H
#define __KEYPAD_H

/* Exported functions --------------------------------------------------------*/
 
/* Configuration ***********************************/
void Keypad_Config(void);

/* Display operation functions ******************************************************/
void Keypad_Columninput(void);
void Keypad_Rowinput(void);
int Keypad_Read(void);

#endif /*__KEYPAD_H */
