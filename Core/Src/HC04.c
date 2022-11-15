/*
 * HC04.c
 *
 *  Created on: May 18, 2022
 *      Author: rangga
 */

#include "stm32f1xx.h"

#define TRIG1_PIN GPIO_PIN_4
#define TRIG2_PIN GPIO_PIN_5
#define TRIG_PORT GPIOB

extern TIM_HandleTypeDef htim2;

struct Ultrasonic {
	uint32_t IC_Val1;
	uint32_t IC_Val2;
	uint32_t Difference;
	uint8_t Is_First_Captured;
	float Distance;
} ultrasonic1;

void delay(uint32_t us) {
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim2)) < us)
		;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {

	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) // if the interrupt source is channel1
			{
		if (ultrasonic1.Is_First_Captured == 0) // if the first value is not captured
				{
			ultrasonic1.IC_Val1 = HAL_TIM_ReadCapturedValue(htim,
			TIM_CHANNEL_1); // read the first value
			ultrasonic1.Is_First_Captured = 1; // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,
					TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (ultrasonic1.Is_First_Captured == 1) // if the first is already captured
				{
			ultrasonic1.IC_Val2 = HAL_TIM_ReadCapturedValue(htim,
			TIM_CHANNEL_1);  // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

			if (ultrasonic1.IC_Val2 > ultrasonic1.IC_Val1) {
				ultrasonic1.Difference = ultrasonic1.IC_Val2
						- ultrasonic1.IC_Val1;
			}

			else if (ultrasonic1.IC_Val1 > ultrasonic1.IC_Val2) {
				ultrasonic1.Difference = (0xffff - ultrasonic1.IC_Val1)
						+ ultrasonic1.IC_Val2;
			}

			ultrasonic1.Distance = ultrasonic1.Difference * .034 / 2;
			ultrasonic1.Is_First_Captured = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1,
					TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim2, TIM_IT_CC1);
		}
	}
}

void Ultrasonic_Read1(void) {

	HAL_GPIO_WritePin(TRIG_PORT, TRIG1_PIN, GPIO_PIN_SET); // pull the TRIG pin HIGH
	delay(10);  // wait for 10 us
	HAL_GPIO_WritePin(TRIG_PORT, TRIG1_PIN, GPIO_PIN_RESET); // pull the TRIG pin low

	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_CC1);

}
