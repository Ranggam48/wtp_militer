/*
 * adc.c
 *
 *  Created on: Sep 23, 2022
 *      Author: rangg
 */

#include"adc.h"
extern ADC_HandleTypeDef hadc1;
extern ADC_ChannelConfTypeDef sConfig;
extern void Error_Handler(void);
uint32_t ADC_VAL[3];

float calCurr(float value) {
	float curr1;
	if (value > 1.60) {
		curr1 = (value - 1.60) * 1000 / 14;
	} else if (value < 1.60) {
		curr1 = (1.60 - value) * 1000 / 14;
	}
	return curr1;
}

void ADC_CH0(void) {

	ADC_ChannelConfTypeDef sConfig = { 0 };

	/** Configure Regular Channel
	 */

	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
}

void ADC_CH6(void) {

	ADC_ChannelConfTypeDef sConfig = { 0 };

	/** Configure Regular Channel
	 */

	sConfig.Channel = ADC_CHANNEL_6;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
}

void ADC_CH7(void) {

	ADC_ChannelConfTypeDef sConfig = { 0 };

	/** Configure Regular Channel
	 */

	sConfig.Channel = ADC_CHANNEL_7;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
}

float Get_OZONE(void) {
	ADC_CH0();
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1000);
	ADC_VAL[0] = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
	return ADC_VAL[0];
}

float Get_Current_EAB(void) {
	ADC_CH6();
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1000);
	ADC_VAL[1] = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
	return calCurr((3.3 * ADC_VAL[1] / 4096));
}

float Get_Volt_EAB(void) {
	ADC_CH7();
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1000);
	ADC_VAL[2] = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
	return (3.3 * ADC_VAL[2] / 4096 / 0.083);
}

