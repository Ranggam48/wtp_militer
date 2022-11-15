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

typedef enum {
	MODE = 0,
	TIMEREAB = 4,
	COUNTBACKWASH = 12,
	COUNTPROCESS = 16,
	LAMAPROCESS = 20,
	LAMAPAKAIEAB = 24,
	TIMERRECOVERYEAB = 28,
	FLAGBACKWASH = 32

} adress;

uint32_t minuteToSecond(float value);
void delay_s(uint8_t value);
void backWash(void);
void process(void);

void setMode(uint32_t value);
void setTimerEAB(uint32_t value);
void setCounter(uint32_t value);
void setCountBackwash(uint32_t value);
void setCountProcess(uint32_t value);
void setLamaProcess(uint32_t value);
void setTimerRcoveryEAB(uint32_t value);
void setLamaPakaiEAB(uint32_t value);
void setFlagBackwash(uint32_t value);

void Task2(void const *argument);
void TaskFlow(void const *argument);
void TaskTimer(void const *argument);

void fungsiInit(void);

#endif /* INC_FUNGSI_H_ */
