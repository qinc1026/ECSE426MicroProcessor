/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program subroutine
	* Author						 : Ashraf Suyyagh
	* Version            : 1.0.0
	* Date							 : January 14th, 2016
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "lis3dsh.h"
#include <math.h>
#include "data.h"
#include "filter.h"
#include "segment.h"
#include "keypad.h"
#include "angle.h"
#include "led.h"
#include "timer.h"

/* Global variables ---------------------------------------------------------*/
__IO float output[3];

/* Private variables ---------------------------------------------------------*/
int test;
float angle;
int pressflag;
int enterflag;
int targetangle;
int temp2;
int presstime;
int Scenario;
int flagTim;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);


int main(void)
{	
  /* MCU Configuration----------------------------------------------------------*/
  flagTim = 0;
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	
  /* Initialize all configured peripherals */  
	Display_Config();
	
	/* Start LED */
	LED_Config();
	
	/* For stopwatch to measure  the time */
	//SET_BIT(DBGMCU->APB1FZ, DBGMCU_APB1_FZ_DBG_TIM3_STOP);
	
	/* Configure the accelerometer */
	//Accelerometer_Config();
	
	/* Start the hardware timer */
	//Timer_Config();
	
	/* Initialize all the flags */
	pressflag = 0;
	enterflag = 0;
	targetangle = 0;
	presstime = 0;
	
	/*while (enterflag == 0){
		temp2 = Keypad_Read();
		if(temp2 != 11 && temp2 != -1 && pressflag == 0 && enterflag == 0 && presstime < 3 && temp2 != 12 && temp2 != -2){
			pressflag = 1;
			presstime++;
			targetangle = temp2 + targetangle*10;
		}
		else if(temp2 == -1){
			pressflag = 0;
		}
		else if(temp2 == 11){
			enterflag = 1;
		}
		else if(temp2 == 12){
			pressflag = 0;
			enterflag = 0;
			targetangle = 0;
			presstime = 0;
		}
	}*/
	
	/* For stopwatch to measure  the time */
	SET_BIT(DBGMCU->APB1FZ, DBGMCU_APB1_FZ_DBG_TIM3_STOP);
	
	/* Configure the accelerometer */
	Accelerometer_Config();
	
	/* Start the hardware timer */
	Timer_Config();
	
	while (1){
		
		temp2 = Keypad_Read();
		if(temp2 != 11 && temp2 != -1 && pressflag == 0 && enterflag == 0 && presstime < 3 && temp2 != 12 && temp2 != -2){
			pressflag = 1;
			presstime++;
			targetangle = temp2 + targetangle*10;
		}
		else if(temp2 == -1){
			pressflag = 0;
		}
		else if(temp2 == 11){
			enterflag = 1;
		}
		else if(temp2 == 12){
			pressflag = 0;
			enterflag = 0;
			targetangle = 0;
			presstime = 0;
		}
		
		angle = Getangle(output);
		if(enterflag == 1){
			Scenario = Scenario_Signal(targetangle, angle);
			if(Scenario == 3){
				Segments_Display(targetangle);
			}
			else{
				Segments_Display(angle);
			}
		}
	}
}

/** System Clock Configuration*/
void SystemClock_Config(void){

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState 			 	= RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState 		= RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM 				= 8;
  RCC_OscInitStruct.PLL.PLLN 				= 336;
  RCC_OscInitStruct.PLL.PLLP 				= RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ 				= 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};

  RCC_ClkInitStruct.ClockType 			= RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5)!= HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};
	
	/*Configures SysTick to provide 1ms interval interrupts. SysTick is already 
	  configured inside HAL_Init, I don't kow why the CubeMX generates this call again*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/* This function sets the source clock for the internal SysTick Timer to be the maximum,
	   in our case, HCLK is now 168MHz*/
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line){
}
#endif

