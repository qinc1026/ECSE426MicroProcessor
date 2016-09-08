/**
  ******************************************************************************
  * @file    filter.h
  * @author  ECSE-426 Group 18
  * @version V1.0.0
  * @date    07-March-2015
  * @brief   Header file containing Kalman filter functions
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FILTER_H
#define __FILTER_H

/* Include data Extension module */
#include "data.h"

/* Exported functions --------------------------------------------------------*/

/* Initialization ***********************************/
void Kstate_Initialization(struct kalman_state* kstate);

/* Filter operation functions ******************************************************/
void Set_Kstate_Parameters(struct kalman_state* kstate, float newQ, float newR, float newX);
float Kalmanfilter_C(struct kalman_state* kstate, float measurement);

#endif /*__FILTER_H */
