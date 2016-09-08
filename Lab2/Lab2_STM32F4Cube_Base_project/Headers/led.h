/**
  ******************************************************************************
  * @file    led.h
  * @author  ECSE-426 Group 18
  * @version V1.0.0
  * @date    17-February-2015
  * @brief   Header file containing LED setup and its basic functions
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

/* Include data Extension module */
 #include <stdint.h>

/* Exported functions --------------------------------------------------------*/
 
/* Configuration ***********************************/
void LED_Config(void);

/* Display operation functions ******************************************************/
void LED_Display(uint32_t turn_off, uint32_t turn_on);
void Overheating_Alarm(float temperature);

#endif /*__LED_H */
