/*
 * HC04.h
 *
 *  Created on: May 18, 2022
 *      Author: rangg
 */

#ifndef INC_HC04_H_
#define INC_HC04_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"


void delay(uint32_t us);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void Ultrasonic_Read1(void);

#endif /* INC_HC04_H_ */
