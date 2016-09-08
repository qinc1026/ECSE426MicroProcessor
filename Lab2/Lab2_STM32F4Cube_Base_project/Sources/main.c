/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : A program which showcases ADC and TIM3 under the new firmware
	                     The ADC
  * Author			   : Ashraf Suyyagh
						 Chuan Qin
						 Wei Wang
  * Version            : 1.2.0
  * Date			   : February 19th, 2016
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include <math.h>
#include "data.h"
#include "filter.h"
#include "segment.h"
#include "led.h"
#include "lcd.h"


/* Volatile variables --------------------------------------------------------*/
extern __IO int TimingDelay;

/* Private variables ---------------------------------------------------------*/
uint32_t ADC_Value;
ADC_HandleTypeDef ADC1_Handle;
struct kalman_state kstate;
int flag2 = 0;
int flag3 = 0;
int cycle = 0;
int initxflag = 0;
float temp, sum, display;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);
void ADC_Config(void);
float Convert_Voltage_To_Celsius(uint32_t voltage);

int main(void)
{
	/* MCU Configuration----------------------------------------------------------*/
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	
	/* Configure the system clock */
	SystemClock_Config();
	/* Configure the ADC */
	ADC_Config();
	/* Start the conversion */
	HAL_ADC_Start(&ADC1_Handle);
	/* Start the 7-segment display */
	Display_Config();
	/* Start the LCD display */
	LCD_Config();
	/* Start LED */
	LED_Config();
	/* Set Kalman Filter and set x be the first value from ADC*/
	Kstate_Initialization(&kstate);
	
	/* Initialize the LCD display before printing */
	Function_Set();
	Display_and_Cursor();
	Set_Entry_Mode();
	/* provide 1us interval interrupt */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000000);

	TimingDelay = 0;
	while (1){
		if(TimingDelay == 0){
			/* 10ms interval interrupt */
			TimingDelay = 10000; 
			HAL_ADC_Start(&ADC1_Handle);
			HAL_ADC_PollForConversion(&ADC1_Handle, 10);
			ADC_Value = HAL_ADC_GetValue(&ADC1_Handle);
			__HAL_ADC_CLEAR_FLAG(&ADC1_Handle, ADC_FLAG_EOC);
			temp = Convert_Voltage_To_Celsius(ADC_Value);
			if (initxflag == 0){
				kstate.x = temp;
				initxflag = 1;
			}
			temp = Kalmanfilter_C(&kstate, temp);

			if(flag3 == 0){
				display = temp;
				flag3 = 1;
				Clear_LCD_Display();
				Set_Display_Address();
				LCD_Display(temp);
			}
			sum += temp;
			flag2++;
			Overheating_Alarm(temp);
			if(flag2 == 10){
				display = sum / 10;
				Clear_LCD_Display();
				Set_Display_Address();
				LCD_Display(display);
				sum = 0;
				flag2 = 0;
			}
			Segments_Display(display);
		}
	}
}

/** System Clock Configuration */
void SystemClock_Config(void){

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType 			= RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState 			 	= RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState 			= RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource 			= RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM 				= 8;
  RCC_OscInitStruct.PLL.PLLN 				= 336;
  RCC_OscInitStruct.PLL.PLLP 				= RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ 				= 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};

  RCC_ClkInitStruct.ClockType 			= RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 		= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 		= RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider 		= RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5)!= HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};
	
	/*Configures SysTick to provide 1ms interval interrupts.*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/* This function sets the source clock for the internal SysTick Timer to be the maximum,
	   in our case, HCLK is now 168MHz*/
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
   * @brief Setup the ADC to sample Channel 16
   * @retval None
   */
void ADC_Config(void){
	//GPIO_InitTypeDef GPIO_Init;
	ADC_ChannelConfTypeDef ADC_Channel;
	ADC1_Handle.Instance = ADC1;
	
	/* Enable the GPIO clock for the used pin and the ADC clock */
	//__GPIOC_CLK_ENABLE();
	__HAL_RCC_ADC1_CLK_ENABLE();
	
	/* Configures the GPIO pin as analog input */
	//GPIO_Init.Pin = GPIO_PIN_1;
	//GPIO_Init.Mode = GPIO_MODE_ANALOG;
	//GPIO_Init.Pull = GPIO_NOPULL;
	//HAL_GPIO_Init(GPIOC, &GPIO_Init);
	
	//HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
	//HAL_NVIC_EnableIRQ(ADC_IRQn);
	
	/* Initialize the configuration of ADC */
	ADC1_Handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV6;
	ADC1_Handle.Init.Resolution = ADC_RESOLUTION_12B;
	ADC1_Handle.Init.ScanConvMode = DISABLE;
	ADC1_Handle.Init.ContinuousConvMode = DISABLE;
	ADC1_Handle.Init.DiscontinuousConvMode = DISABLE;
	ADC1_Handle.Init.NbrOfDiscConversion = 0;
	ADC1_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADC1_Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	ADC1_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC1_Handle.Init.NbrOfConversion = 1;
	ADC1_Handle.Init.DMAContinuousRequests = DISABLE;
	ADC1_Handle.Init.EOCSelection = DISABLE;
	HAL_ADC_Init(&ADC1_Handle);
	
	/* Initialize the ADC Channel configuration */
	ADC_Channel.Channel = ADC_CHANNEL_16;
	ADC_Channel.Rank = 1;
	ADC_Channel.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	ADC_Channel.Offset = 0;
	
	/* Set up the ADC Channel */
	if (HAL_ADC_ConfigChannel(&ADC1_Handle, &ADC_Channel) != HAL_OK) {
		Error_Handler(ADC_CH_CONFIG_FAIL);
	}
}

/**
   * @brief Calculates the temperature using the formula as following:
			Temperature(in C) = {(Vsense - V25) / Avg_Slope} + 25
			Vsense (Vsense value at 25 degree Celsius) = 0.76 V
			Avg_Slope (average slope of the temperature vs. Vsense curve) = 2.5 mV
   * @param  voltage: the reading value obtained by ADC
   * @retval value in temperature format in Celsius
   */
float Convert_Voltage_To_Celsius(uint32_t voltage){
	float Temperature = (float)voltage;
	Temperature *= 3000;
	Temperature /= 0xfff;			/* Reading in mV */
	Temperature /= (float)1000.0;	/* Reading in Volts */
	Temperature -= (float)0.760;	/* Substract the reference voltage at 25 degree Celsius */
	Temperature /= (float)0.0025;	/* Divide by slope 2.5mV */
	Temperature += (float)25.0;		/* Add the 25 degree Celsius */
	
	return Temperature;
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
