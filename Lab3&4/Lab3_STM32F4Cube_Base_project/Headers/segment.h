/**
  ******************************************************************************
  * @file    segment.h
  * @author  ECSE-426 Group 18
  * @version V1.0.0
  * @date    07-March-2016
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
void Segments_DisplayXYY(float degree);
void Segments_DisplayXXY(float degree);
void Segments_DisplayXXX(float degree);
void Segments_Display(float degree);

#endif /*__SEGMENT_H */
