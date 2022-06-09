/*
 * fungsi.h
 *
 *  Created on: May 24, 2022
 *      Author: rangg
 */

#ifndef INC_FUNGSI_H_
#define INC_FUNGSI_H_

#include "stm32f1xx_hal.h"

#define ADDRESS 0x0801F840 // address EEPROM

uint32_t minuteToSecond(float value);
void delay_s(uint8_t value);
void backWash(void);
void process1(void);
void process2(void);
void process3(void);

void setMode(uint32_t value);
void setTimerEAB(uint32_t value);
void setCounter(uint32_t value);

void Task2(void const *argument);
void TaskFlow(void const *argument);
void TaskTimer(void const *argument);

void fungsiInit(void);

#endif /* INC_FUNGSI_H_ */
