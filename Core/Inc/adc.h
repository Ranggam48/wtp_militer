/*
 * adc.h
 *
 *  Created on: Sep 23, 2022
 *      Author: rangg
 */

#include "stm32f1xx.h"

#ifndef INC_ADC_H_
#define INC_ADC_H_

void ADC_CH0(void);
void ADC_CH6(void);
void ADC_CH7(void);

float Get_OZONE(void);
float Get_Current_EAB(void);
float Get_Volt_EAB(void);

#endif /* INC_ADC_H_ */
