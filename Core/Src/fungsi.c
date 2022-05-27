/*
 * fungsi.c
 *
 *  Created on: May 24, 2022
 *      Author: rangg
 */

// kumpulan include
#include "fungsi.h"
#include "flash.h"
#include "string.h"
#include "com_can.h"
#include "FLASH.h"
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

// kumpulan variable dan penanda/flag

uint8_t flowCheck, backWashCheck, flag, level, flagBackwash;
uint8_t selfID = 1;
uint8_t data[13] = "Hallo Rangga\n";

uint32_t buff[1] = { 0 };
uint32_t timerEAB[1] = { 0 };
uint32_t mode[1] = { 0 };
uint32_t counter[1] = { 0 };
uint32_t timerBackwash[1] = { 0 };

float volume, flow;

// kumpulan fungsi

uint32_t minuteToSecond(float value) {

	return value * 60 * 1000;
}

void process1(void) // opsi sensor ultrasonic
{
	while (timerEAB != 0) {
		EAB(ON);
	}
	EAB(OFF);
	osDelay(minuteToSecond(1));

	label1: comm_can_db_signal(0, 2); // megirim sinyal untuk memutarkan HVRDF

	osDelay(minuteToSecond(0.3)); // label1 untuk mengembalikan program ke awal

	Control_Valve_1(ON);
	Pump_1(ON);
	Ozone(ON);
	Compressor(ON);
	Pump_2(ON);

	flowCheck = 1; // fungsi opsional ketika tidak menggunakan sensor ultarasonic tanda untuk memberi tahu bahwa air sedang mengalir

	osDelay(minuteToSecond(0.3));

	while (volume > 3) {
		osDelay(5000);
		if (flow < 1) {
			osDelay(minuteToSecond(1 / 3)); // delay untuk menunggu air pada rdf kosong
			comm_can_db_signal(0, 3); // mengirim sinyal ke driver untuk backwash
			break;
		}
	}

	Control_Valve_1(OFF);
	osDelay(minuteToSecond(1000)); // delay menghabiskan air pada selang
	Pump_1(OFF);
	osDelay(minuteToSecond(1 / 6)); // delay menghabiskan air pada selang
	Ozone(OFF);
	Compressor(OFF);
	Pump_2(OFF);

	while (!backWashCheck) {
		osDelay(5000);
	}

	if (volume > 3) {
		goto label1;
	}

	flowCheck = 0;
	mode[0] = 0;
}

void process2(void) // opsi sensor water level
{
	while (timerEAB > 0) {
		EAB(ON);
	}

	EAB(OFF);
	osDelay(minuteToSecond(1));
	label2: comm_can_db_signal(0, 2); // megirim sinyal untuk memutarkan HVRDF

	osDelay(minuteToSecond(0.5));

	Control_Valve_1(ON);
	Pump_1(ON);
	Ozone(ON);
	Compressor(ON);
	Pump_2(ON);

	osDelay(minuteToSecond(0.3));

	while (level) {
		osDelay(5000);
		if (flow < 1) {
			osDelay(minuteToSecond(1 / 3)); // delay untuk menunggu air pada rdf kosong
			comm_can_db_signal(0, 3); // mengirim sinyal ke driver untuk backwash
			break;
		}
	}

	Control_Valve_1(OFF);
	osDelay(1000); // delay menghabiskan air pada selang
	Pump_1(OFF);
	osDelay(minuteToSecond(1 / 6)); // delay menghabiskan air pada selang
	Ozone(OFF);
	Compressor(OFF);
	Pump_2(OFF);

	while (!backWashCheck) { //menunggu sinyal dari driver bahwa backwash telah selesai
		osDelay(5000);
	}

	if (level) {
		goto label2;
		// kembali ke awal
	}
	mode[0] = 0;
	timerEAB[0] = 1800;
}

void process3(void) { // opsi backwash ditrigger counter

	while (timerEAB > 0) {
		EAB(ON);
	}

	EAB(OFF);
	osDelay(minuteToSecond(1));
	label2: comm_can_db_signal(0, 2); // megirim sinyal untuk memutarkan HVRDF

	osDelay(minuteToSecond(0.5));

	Control_Valve_1(ON);
	Pump_1(ON);
	Ozone(ON);
	Compressor(ON);
	Pump_2(ON);

	osDelay(minuteToSecond(0.3));

	while (level) {
		osDelay(5000);
		if (flow < 1) {
			osDelay(minuteToSecond(1 / 3)); // delay untuk menunggu air pada rdf kosong
			comm_can_db_signal(0, 4); // mengirim sinyal ke driver untuk backwash
			break;
		}
	}

	Control_Valve_1(OFF);
	osDelay(1000); // delay menghabiskan air pada selang
	Pump_1(OFF);
	osDelay(minuteToSecond(1 / 6)); // delay menghabiskan air pada selang
	Ozone(OFF);
	Compressor(OFF);
	Pump_2(OFF);

	flagBackwash = 1; // untuk memulai menghitung mundur timerBackwash pada Task Timer
	timerBackwash[0] = 30; // timer untuk backwash 30 detik

	while (timerBackwash > 0) { //menunggu sinyal dari driver bahwa backwash telah selesai
		osDelay(5000);
	}

	if (level) {
		goto label2;
		// kembali ke awal
	}
	mode[0] = 0;
	timerEAB[0] = 1800;
}

void setMode(uint32_t value) {

	Flash_Write_Data(EEPROMMode, (uint32_t*) value, 1);
}

void setTimerEAB(uint32_t value) {

	Flash_Write_Data(EEPROMMode, (uint32_t*) value, 1);

}

void setCounter(uint32_t value) {

	Flash_Write_Data(EEPROMCounter, (uint32_t*) value, 1);

}

// kumpulan task freeRTOS

void Task2(void const *argument) {
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for (;;) {

		if (mode[0] == 1) // mode 1 untuk opsi menggunakan sensor ultrasonic
				{

		}

		else if (mode[0] == 2) // mode 2 untuk opsi menggunakan sensor water level
				{

		}

		else if (mode[0] == 3) // mode 2 untuk opsi menggunakan sensor water level
				{

		}

		else {

		}
		osDelay(500);
	}
	/* USER CODE END 5 */
}

void TaskTimer(void const *argument) {
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for (;;) {

		if (mode[0] == 2 || mode[0] == 1 || mode[0] == 3) { // jika mode bernilai 1/2/3 maka timer eab akan mulai hitung mundur
			timerEAB[0]--;
			setTimerEAB(timerEAB[0]);
		}

		else if (flagBackwash == 1) {
			timerBackwash[0]--;
		}
		osDelay(1000);
	}
	/* USER CODE END 5 */
}

void fungsiInit(void) {

	gpio_init();

	Flash_Read_Data(EEPROMMode, buff, 1); // membaca eeprom mode
	mode[0] = buff[0];

	Flash_Read_Data(EEPROMTimerEAB, buff, 1); // membaca eeprom timerEAB
	timerEAB[0] = buff[0];

	Flash_Read_Data(EEPROMCounter, buff, 1); // membaca eeprom counter
	counter[0] = buff[0];

	osThreadDef(Task2, Task2, osPriorityNormal, 0, 256);
	Task2Handle = osThreadCreate(osThread(Task2), NULL);

	osThreadDef(TaskTimer, TaskTimer, osPriorityNormal, 0, 256);
	TaskTimerHandle = osThreadCreate(osThread(TaskTimer), NULL);
}

