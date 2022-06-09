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
osThreadId TaskFlowHandle;

// kumpulan variable dan penanda/flag

uint8_t flowCheck, backWashCheck, flag, level, flagBackwash, coba, sensor;
uint8_t selfID = 1;
uint8_t data[13] = "Hallo Rangga\n";

uint32_t buffEAB;
uint32_t timerEAB = 600;
uint32_t counter;
uint32_t mode;
uint32_t timerBackwash;
uint32_t dataWrite[10];
uint32_t dataRead[10];

float volume, flow;
int signalCounter;

// kumpulan fungsi

uint32_t minuteToSecond(float value) {

	return value * 60 * 1000;

}

void delay_s(uint8_t value) {
	osDelay(value * 1000);
}

void backWash(void) {

	Control_Valve_1(OFF);
	//osDelay(1000); // delay menghabiskan air pada selang
	Pump_1(OFF);
	delay_s(2); // delay menghabiskan air pada selang
	Ozone(OFF);
	Compressor(OFF);
	//Pump_2(OFF);

	delay_s(10); // delay untuk menunggu air pada rdf kosong
	comm_can_set_duty(0, 0);
	delay_s(10); // delay agar motor tidak menyentak
	comm_can_db_signal(0, 0); // mengirim sinyal ke driver untuk mematikan rdf
	comm_can_db_signal(0, 4); // mengirim sinyal ke driver untuk backwash
	delay_s(3);
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

//void process1(void) // opsi sensor ultrasonic
//{
//
//	while (timerEAB != 0) {
//		EAB(ON);
//	}
//	EAB(OFF);
//	osDelay(minuteToSecond(1));
//
//	label1: comm_can_db_signal(0, 2); // megirim sinyal untuk memutarkan HVRDF
//
//	osDelay(minuteToSecond(0.3)); // label1 untuk mengembalikan program ke awal
//
//	Control_Valve_1(ON);
//	Pump_1(ON);
//	Ozone(ON);
//	Compressor(ON);
//	Pump_2(ON);
//
//	flowCheck = 1; // fungsi opsional ketika tidak menggunakan sensor ultarasonic tanda untuk memberi tahu bahwa air sedang mengalir
//
//	osDelay(minuteToSecond(0.3));
//
//	while (volume > 3) {
//		osDelay(5000);
//		if (flow < 1) {
//			osDelay(minuteToSecond(1 / 3)); // delay untuk menunggu air pada rdf kosong
//			comm_can_db_signal(0, 3); // mengirim sinyal ke driver untuk backwash
//			break;
//		}
//	}
//
//	Control_Valve_1(OFF);
//	osDelay(minuteToSecond(1000)); // delay menghabiskan air pada selang
//	Pump_1(OFF);
//	osDelay(minuteToSecond(1 / 6)); // delay menghabiskan air pada selang
//	Ozone(OFF);
//	Compressor(OFF);
//	Pump_2(OFF);
//
//	while (!backWashCheck) {
//		osDelay(5000);
//	}
//
//	if (volume > 3) {
//		goto label1;
//	}
//
//	flowCheck = 0;
//	buffMode[0] = 0;
//}

void process2(void) // opsi sensor water level
{
	//timerEAB[0] = 1800;

	if (!level) {
		mode = 0;
		return;
	}

	while (timerEAB > 0) {
		EAB(ON);
		osDelay(10);
	}

	EAB(OFF);
	delay_s(30);

	label2:

	comm_can_set_duty(0, 90);
	comm_can_db_signal(0, 2); // megirim sinyal untuk memutarkan HVRDF

	delay_s(15); //delay untuk menunggu rdf putaran penuh

	Control_Valve_1(ON);
	Pump_1(ON);
	Ozone(ON);
	Compressor(ON);
	Pump_2(ON);

	delay_s(30);

	while (1) {

		osDelay(1000);
		if (flow < 4.0 // || !level
				) {
			backWash();
			break;
		}
	}

	if (level) {
		goto label2;
		// kembali ke awal
	}

	Pump_2(OFF);
	mode = 0;
	timerEAB = 120;
}

//void process3(void) { // opsi backwash ditrigger counter
//
//	while (timerEAB[0] > 0) {
//		EAB(ON);
//	}
//
//	EAB(OFF);
//	osDelay(minuteToSecond(1));
//	label2: comm_can_db_signal(0, 2); // megirim sinyal untuk memutarkan HVRDF
//
//	osDelay(minuteToSecond(0.5));
//
//	Control_Valve_1(ON);
//	Pump_1(ON);
//	Ozone(ON);
//	Compressor(ON);
//	Pump_2(ON);
//
//	osDelay(minuteToSecond(0.3));
//
//	while (1) {
//		osDelay(5000);
//		if (flow < 1) {
//			osDelay(minuteToSecond(1 / 3)); // delay untuk menunggu air pada rdf kosong
//			comm_can_db_signal(0, 4); // mengirim sinyal ke driver untuk backwash
//			flagBackwash = 1; // untuk memulai menghitung mundur timerBackwash pada Task Timer
//			timerBackwash[0] = 30; // timer untuk backwash 30 detik
//			break;
//		}
//	}
//
//	Control_Valve_1(OFF);
//	osDelay(1000); // delay menghabiskan air pada selang
//	Pump_1(OFF);
//	osDelay(minuteToSecond(1 / 6)); // delay menghabiskan air pada selang
//	Ozone(OFF);
//	Compressor(OFF);
//	Pump_2(OFF);
//	Pump_3(ON);
//
//	uint8_t i = 0;
//
//	while (i <= 30) { //menunggu sinyal dari driver bahwa backwash telah selesai
//		osDelay(1000);
//		i++;
//	}
//	Pump_3(OFF);
//
//	flagBackwash = 0;
//
//	if (level) {
//		goto label2;
//		// kembali ke awal
//	}
//	buffMode[0] = 0;
//	timerEAB[0] = 1800;
//}

void setMode(uint32_t value) {

	dataWrite[0] = value;
	Flash_Write_Data(ADDRESS, (uint32_t*) dataWrite, 10);

}

void setTimerEAB(uint32_t value) {

	dataWrite[1] = value;
	Flash_Write_Data(ADDRESS, (uint32_t*) dataWrite, 10);

}

void setCounter(uint32_t value) {

	dataWrite[2] = value;
	Flash_Write_Data(ADDRESS, (uint32_t*) value, 10);

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
			//process3();
		}

		else if (mode == 0) {
			timerEAB = 120;
		}
		osDelay(1000);
		coba++;

	}
	/* USER CODE END 5 */
}

void TaskTimer(void const *argument) {
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for (;;) {

		if (mode == 2 || mode == 1 || mode == 3) { // jika mode bernilai 1/2/3 maka timer eab akan mulai hitung mundur
			timerEAB--;
		}

		else if (flagBackwash == 1) {
			timerBackwash--;
		}
		flow = signalCounter / 7.5;
		signalCounter = 0;

		osDelay(1000);
		setTimerEAB(timerEAB);
		//comm_can_send_buffer(7, data, sizeof(data), 0);

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
		level = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
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

	Flash_Read_Data(ADDRESS, dataRead, 10); // membaca eeprom timerEAB

	mode = dataRead[0];
	timerEAB = dataRead[1];

	mode = 3;

	osThreadDef(TaskTimer, TaskTimer, osPriorityNormal, 0, 128);
	TaskTimerHandle = osThreadCreate(osThread(TaskTimer), NULL);

	osThreadDef(Task2, Task2, osPriorityNormal, 0, 128);
	Task2Handle = osThreadCreate(osThread(Task2), NULL);

	osThreadDef(TaskFlow, TaskFlow, osPriorityBelowNormal, 0, 128);
	TaskFlowHandle = osThreadCreate(osThread(TaskFlow), NULL);
}

