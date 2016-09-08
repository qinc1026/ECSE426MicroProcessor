/*******************************************************************************
  * @file    Thread_Keypad.c
  * @author  Chuan Qin, Wei Wang
  * @version V1.0.0
  * @date    17-March-2016
  * @brief   This file initializes keypad as input and output,
  *			 			implements the keypad thread which reads input and generates outputs,
  *			 			and function which creates and starts the thread	
  ******************************************************************************
  */
	
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "data.h"

void Thread_Keypad (void const *argument);                 // thread function
osThreadId tid_Thread_Keypad;                              // thread id
osThreadDef(Thread_Keypad, osPriorityAboveNormal, 1, 0);

/* Extern variables	----------------------------------------------------------*/
extern int flagTim2;
extern osThreadId tid_Thread_ADC;
extern osThreadId tid_Thread_Accelerometer;


/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitD;
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
int temp2;
int temp3;
int pressflag;
int keypadresult;

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
			case 0xE7:		//*
				return 14;
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
			case 0xB7:		//#
				return 15;
			case 0x7E:		//A
				return 10;
			case 0x7D:		//B
				return 11;
			case 0x7B:		//C
				return 12;
			case 0x77:		//D
				return 13;
		}
	}
	else{
		return -1;
	}
	
}

/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_Keypad (void) {

  tid_Thread_Keypad = osThreadCreate(osThread(Thread_Keypad ), NULL); // Start Keypad_Thread
  if (!tid_Thread_Keypad) return(-1); 
  return(0);
}

/*----------------------------------------------------------------------------
 *      Thread  'Keypad_Thread': Read the pressed key
 *---------------------------------------------------------------------------*/
	void Thread_Keypad (void const *argument) {
		pressflag = 0;
		keypadresult = -1;
		temp2 = -1;
		while(1){
			if (flagTim2 > 50 || pressflag == 1){
					flagTim2 = 0;
					temp2 = Keypad_Read();
					if (temp2 == 1 || temp2 == 2 || temp2 == 10 || temp2 == 11){
						pressflag = 1;
						temp3 = temp2;
					}
					else if (temp2 ==-1){
						pressflag = 0;
						keypadresult = temp3;
					}
				}
			if (keypadresult == 10){
				osSignalSet(tid_Thread_ADC, 0x01);
			}
			else if (keypadresult == 11 || keypadresult == 1 || keypadresult == 2){
				osSignalSet(tid_Thread_Accelerometer, 0x02);
			}
			osDelay(SLEEP);
			//TIM3_Delay(SLEEP);
			}
	}

/*----------------------------------------------------------------------------
 *      Initialize the GPIO associated with the keypad columns and rows
 *---------------------------------------------------------------------------*/
	void initializeKeypad_IO (void){
	
	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitD.Pin = Col1 | Col2 | Col3 | Col4 | Row1 | Row2 | Row3 | Row4;
	GPIO_InitD.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitD.Pull = GPIO_PULLUP;
	GPIO_InitD.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitD);
	}
	
/*----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/
