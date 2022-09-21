/*
 * fungsi.c
 *
 *  Created on: May 24, 2022
 *      Author: rangg
 */

// kumpulan include
#include "fungsi.h"
#include "EEPROM.h"
#include "string.h"
#include "com_can.h"
#include <gpio.h>
#include <HC04.h>
#include "FreeRTOS.h"
#include"FreeRTOSConfig.h"
#include "cmsis_os.h"

typedef enum {
	ON = 0, OFF
} Pin_Status;

// kumpulan handle task freeRTOS

osThreadId Task2Handle;
osThreadId TaskTimerHandle;
osThreadId TaskFlowHandle;
osThreadId TaskButtonHandle;

// kumpulan variable dan penanda/flag

uint8_t flowCheck, backWashCheck, flag, level, flagBackwash, flagTimerEAB,
		flagRecoveryEAB, cobaTask2, cobaTaskTimer, cobaTaskButton, cobaTaskFlow,
		sensor;

uint8_t selfID = 1;
uint8_t dataCan[8];
uint8_t p;

uint32_t buffEAB;
uint32_t timerEAB;
uint32_t timerRecoveryEAB;
uint32_t counter;
uint32_t mode;
uint32_t timerBackwash;
uint16_t lamaMampet;
uint32_t countBackwash;
uint32_t countProcess;
uint32_t lamaProcess;
uint32_t lamaPakaiEAB;
uint32_t dataWrite[10];
uint32_t dataRead[10];

float volume, flow; // variabel untuk flow sensor
int signalCounter, buttonCounter; // variabel untuk counter hall sensor dan button

// kumpulan fungsi

uint32_t minuteToSecond(float value) {

	return value * 60 * 1000;

}

void delay_s(uint8_t value) {
	osDelay(value * 1000);
}

void delay_m(uint8_t value) {
	osDelay(value * 1000 * 60);
}

void backWash(void) {

	Control_Valve_1(OFF);
	//osDelay(1000); // delay menghabiskan air pada selang
	Pump_1(OFF);
	delay_s(2); // delay menghabiskan air pada selang
	Ozone(OFF);
	Compressor(OFF);
	flagBackwash = 0; // memulai rekam waktu filtrasi rdf sampai tersumbat
	//Pump_2(OFF);

	delay_s(10);
	comm_can_db_signal(0, 0); // mengirim sinyal ke driver untuk mematikan rdf
	delay_s(5);
	comm_can_db_signal(0, 4); // mengirim sinyal ke driver untuk backwash
	delay_s(1);
	Pump_3(ON); // pompa backwash menyala

	uint8_t i = 0;

	while (i <= 30) { //menunggu sinyal dari driver bahwa backwash telah selesai
		osDelay(1000);
		i++;
	}
	Pump_3(OFF); //pompa backwash mati
	comm_can_db_signal(0, 0); // mengirim sinyal ke driver untuk mematikan rdf
	delay_s(5);
}

void process(void) {

	delay_s(2);

//	if (level == 1) {
//		mode = 0;
//		comm_can_db_signal(0, 0);
//		return;
//	}

	flagTimerEAB = 1; // flag untuk memulai timer EAB

	while (timerEAB < 1200) {
		EAB(ON);
		delay_s(1);
		if (mode == 0) {
			goto end;
		}
	}
	EAB(OFF);

	flagTimerEAB = 0; // flag untuk menghentikan timer EAB

	flagRecoveryEAB = 1; // flag untuk recover EAB

	while (timerRecoveryEAB < 600) {
		delay_s(1);
	}

	flagRecoveryEAB = 0; // flag untuk recover EAB

	label2: // balik lagi ke sini jika air masih penuh

	comm_can_set_duty(0, 95);
	comm_can_db_signal(0, 2); // megirim sinyal untuk memutarkan HVRDF

	delay_s(7); //delay untuk menunggu rdf putaran penuh

	Control_Valve_1(ON);
	Pump_1(ON);
	Ozone(ON);
	Compressor(ON);
	Pump_2(ON);

	flagBackwash = 1; // penanda start backwash untuk perhitungan lamanya filter tersumbat
	lamaMampet = 0; // reset waktu mampet filter

	delay_s(30);

	while (1) {

		osDelay(3000);

		if (flow < 4 //|| level == 1
		|| mode == 0) {
			countBackwash++;
			delay_s(1);
			setCountBackwash(countBackwash);
			dataCan[0] = countBackwash;
			dataCan[1] = lamaMampet & 0x00ff;
			dataCan[2] = (lamaMampet & 0xff00) >> 8;
			comm_can_transmit_eid(4, dataCan, 8);
			Ozone(OFF);
			Compressor(OFF);
			backWash();
			break;
		}
	}

	if (mode == 0) {
		goto end;
	}

//	if (level == 1) {
//		goto label2;
//		// kembali ke awal
//	}
	goto label2;

	end:

	EAB(OFF);
	delay_s(3);
	Pump_2(OFF);
	mode = 0;
	countProcess++;
	setCountProcess(countProcess);

	flagTimerEAB = 0;
	flagRecoveryEAB = 0;

}

void setMode(uint32_t value) {

	EEPROM_Write_NUM(1, MODE, value);
}

void setTimerEAB(uint32_t value) {

	EEPROM_Write_NUM(1, TIMEREAB, value);

}

void setLamaPakaiEAB(uint32_t value) {

	EEPROM_Write_NUM(1, LAMAPAKAIEAB, value);

}

void setTimerRcoveryEAB(uint32_t value) {

	EEPROM_Write_NUM(1, TIMERRECOVERYEAB, value);

}

void setCounter(uint32_t value) {

//	dataWrite[2] = value;
//	Flash_Write_Data(ADDRESS, (uint32_t*) value, 10);

}

void setCountBackwash(uint32_t value) {

	EEPROM_Write_NUM(1, COUNTBACKWASH, value);

}

void setCountProcess(uint32_t value) {

	EEPROM_Write_NUM(1, COUNTPROCESS, value);

}

void setLamaProcess(uint32_t value) {

	EEPROM_Write_NUM(1, LAMAPROCESS, value);

}

// kumpulan task freeRTOS

void Task2(void const *argument) {
	/* USER CODE BEGIN 5 */

	/* Infinite loop */
	for (;;) {

		if (mode == 1) // mode 1 untuk opsi menggunakan sensor ultrasonic
				{
			process();
		}

		else if (mode == 2) // mode 2 untuk opsi menggunakan sensor water level
				{

		}

		else if (mode == 3) // mode 2 untuk opsi menggunakan sensor water level
				{
		}

		else if (mode == 0) {
			lamaProcess = 0;
			setLamaProcess(lamaProcess);
			timerEAB = 0;
			setTimerEAB(timerEAB);
			timerRecoveryEAB = 0;
			setTimerRcoveryEAB(timerRecoveryEAB);
		}
		osDelay(1000);
		//cobaTask2++;

	}
	/* USER CODE END 5 */
}

void TaskTimer(void const *argument) {
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for (;;) {

		if (flagTimerEAB == 1) { // jika mode bernilai 1 maka timer eab akan mulai hitung

			timerEAB++;
			setTimerEAB(timerEAB);
			if (timerEAB % 60 == 0) {
				lamaPakaiEAB++;
				setLamaPakaiEAB(lamaPakaiEAB);
			}

		}

		if (flagRecoveryEAB == 1) {
			timerRecoveryEAB++;
			setTimerRcoveryEAB(timerRecoveryEAB);
		}

		if (flagBackwash == 1) {
			lamaMampet++;
		}

		if (mode == 2) {
			lamaProcess++;
			setLamaProcess(lamaProcess);
		}

		flow = signalCounter / 7.5;
		signalCounter = 0;
		cobaTaskTimer++;
		osDelay(1000);

	}
	/* USER CODE END 5 */
}

void TaskFlow(void const *argument) {
	/* USER CODE BEGIN 5 */
	uint8_t stateSensorNow = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
	uint8_t stateSensorPrv = -1;
	/* Infinite loop */
	for (;;) {
		stateSensorNow = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
		level = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4));
		if (stateSensorNow != stateSensorPrv) {
			signalCounter++;
			stateSensorPrv = stateSensorNow;
		}
		//cobaTaskFlow++;
		osDelay(1);
	}
	/* USER CODE END 5 */
}

void TaskButton(void const *argument) {
	/* USER CODE BEGIN 5 */
	uint8_t stateButtonNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	uint8_t stateButtonPrv = 1;

	/* Infinite loop */
	for (;;) {
		stateButtonNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
		if (stateButtonNow == 1 && stateButtonPrv == 0) {
			mode++;
			if (mode > 1) {
				mode = 0;
			}
			stateButtonPrv = 1;
		} else if (stateButtonNow == 0) {
			stateButtonPrv = 0;
		}
		osDelay(10);
		//cobaTaskButton++;
		setMode(mode);
	}
	/* USER CODE END 5 */
}

void fungsiInit(void) {

	gpio_init();

	// pembacaan eeprom
	mode = EEPROM_Read_NUM(1, MODE);
	HAL_Delay(10);
	timerEAB = EEPROM_Read_NUM(1, TIMEREAB);
	HAL_Delay(10);
	countBackwash = EEPROM_Read_NUM(1, COUNTBACKWASH);
	HAL_Delay(10);
	countProcess = EEPROM_Read_NUM(1, COUNTPROCESS);
	HAL_Delay(10);
	lamaProcess = EEPROM_Read_NUM(1, LAMAPROCESS);
	HAL_Delay(10);
	lamaPakaiEAB = EEPROM_Read_NUM(1, LAMAPAKAIEAB);
	HAL_Delay(10);
	timerRecoveryEAB = EEPROM_Read_NUM(1, TIMERRECOVERYEAB);
	HAL_Delay(10);

	//mode = 2;
//	setMode(mode);
//	setLamaPakaiEAB(630);
//	setCountBackwash(154);

	osThreadDef(TaskTimer, TaskTimer, osPriorityNormal, 0, 64);
	TaskTimerHandle = osThreadCreate(osThread(TaskTimer), NULL);

	osThreadDef(Task2, Task2, osPriorityNormal, 0, 64);
	Task2Handle = osThreadCreate(osThread(Task2), NULL);

	osThreadDef(TaskFlow, TaskFlow, osPriorityBelowNormal, 0, 64);
	TaskFlowHandle = osThreadCreate(osThread(TaskFlow), NULL);

	osThreadDef(TaskButton, TaskButton, osPriorityBelowNormal, 0, 64);
	TaskButtonHandle = osThreadCreate(osThread(TaskButton), NULL);
}

