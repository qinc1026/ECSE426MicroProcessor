/**
  ******************************************************************************
  * @file    segment.h
  * @author  ECSE-426 Group 18
  * @version V1.0.0
  * @date    17-February-2015
  * @brief   Header file containing four digit seven-segment setup and its basic functions
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SEGMENT_H
#define __SEGMENT_H

/* Exported functions --------------------------------------------------------*/
 
/* Configuration ***********************************/
void Display_Config(void);

/* Display operation functions ******************************************************/
void One_Segment_Display(int number);
void Digit_Segments_Display(char digit);
void Segments_Display(float temperature);

#endif /*__SEGMENT_H */
