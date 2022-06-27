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

// kumpulan variable dan penanda/flag

uint8_t flowCheck, backWashCheck, flag, level, flagBackwash, flagTimerEAB,
		flagRecoveryEAB, coba, sensor;

uint8_t selfID = 1;
uint8_t dataCan[8];

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
int signalCounter; // variabel untuk counter hall sensor

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

void process2(void) // opsi sensor water level
{
	delay_s(2);

//	if (level == 1) {
//		mode = 0;
//		comm_can_db_signal(0, 0);
//		return;
//	}

	flagTimerEAB = 1;
	while (timerEAB < 1800) {
		EAB(ON);
		delay_s(1);
	}
	EAB(OFF);
	flagTimerEAB = 0;

	flagRecoveryEAB = 1;
	while (timerRecoveryEAB < 600) {
		delay_s(1);
	}
	flagRecoveryEAB = 0;

	label2:

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
		if (flow < 4 || level == 1) {
			countBackwash++;
			delay_s(1);
			setCountBackwash(countBackwash);
			dataCan[0] = countBackwash;
			dataCan[1] = lamaMampet & 0x00ff;
			dataCan[2] = (lamaMampet & 0xff00) >> 8;
			comm_can_transmit_eid(4, dataCan, 8);
			backWash();
			break;
		}
	}

	if (level == 1) {
		goto label2;
		// kembali ke awal
	}

	delay_s(3);
	Pump_2(OFF);
	mode = 0;
	countProcess++;
	setCountProcess(countProcess);
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

		}

		else if (mode == 2) // mode 2 untuk opsi menggunakan sensor water level
				{
			process2();
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
		coba++;

	}
	/* USER CODE END 5 */
}

void TaskTimer(void const *argument) {
	/* USER CODE BEGIN 5 */
	uint32_t i = 0;
	/* Infinite loop */
	for (;;) {

		if (flagTimerEAB == 1) { // jika mode bernilai 1 maka timer eab akan mulai hitung
			timerEAB++;
			i = timerEAB;
			setTimerEAB(timerEAB);
			if (i % 60 == 0 && i > 60) {
				lamaPakaiEAB++;
				setLamaPakaiEAB(lamaPakaiEAB);
				i = 0;
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
		osDelay(1);
	}
	/* USER CODE END 5 */
}

void fungsiInit(void) {

	gpio_init();

	// pembacaan eeprom
	mode = EEPROM_Read_NUM(1, MODE);
	timerEAB = EEPROM_Read_NUM(1, TIMEREAB);
	countBackwash = EEPROM_Read_NUM(1, COUNTBACKWASH);
	countProcess = EEPROM_Read_NUM(1, COUNTPROCESS);
	lamaProcess = EEPROM_Read_NUM(1, LAMAPROCESS);
	lamaPakaiEAB = EEPROM_Read_NUM(1, LAMAPAKAIEAB);
	timerRecoveryEAB = EEPROM_Read_NUM(1, TIMERRECOVERYEAB);

	mode = 2;
	setMode(mode);
	setLamaPakaiEAB(600);
	setCountBackwash(102);

	osThreadDef(TaskTimer, TaskTimer, osPriorityNormal, 0, 128);
	TaskTimerHandle = osThreadCreate(osThread(TaskTimer), NULL);

	osThreadDef(Task2, Task2, osPriorityNormal, 0, 128);
	Task2Handle = osThreadCreate(osThread(Task2), NULL);

	osThreadDef(TaskFlow, TaskFlow, osPriorityBelowNormal, 0, 128);
	TaskFlowHandle = osThreadCreate(osThread(TaskFlow), NULL);
}

