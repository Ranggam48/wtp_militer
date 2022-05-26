///*
// * terminal.c
// *
// *  Created on: Feb 9, 2022
// *      Author: rangga
// */
//
//#include "terminal.h"
//#include "main.h"
////#include "usbd_cdc_if.h"
//#include <stdio.h>
//#include <string.h>
//#include "stdarg.h"
//
//void terminal_proses(uint8_t *str) {
//	enum {
//		kMaxArgs = 64
//	};
//	int argc = 0;
//	char *argv[kMaxArgs];
//
//	char *p2 = strtok(str, " ");
//	while (p2 && argc < kMaxArgs) {
//		argv[argc++] = p2;
//		p2 = strtok(0, " ");
//	}
//	if (argc == 0) {
//
//		command_printf("tidak ada perintah\n");
//	}
//
////    else if(strcmp(argv[0], "off") == 0)
////    {
//////    	command_printf("angka = %d\n", 5);
//////    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
////    	palSetPad(GPIOC, GPIO_PIN_13);
////    	command_printf("led off\n");
////     }
////
////    else if(strcmp(argv[0], "on") == 0)
////    {
//////    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
////    	palClearPad(GPIOC, GPIO_PIN_13);
////    	command_printf("led on\n");
////    }
//
//	/***********************GPIO***********************/
//
//	else if (strcmp(argv[0], "cmd") == 0) {
//		command_printf("EAB_ON --->  menyalakan EAB\n"
//				"EAB_OFF ---> mematikan EAB\n"
//				"CV1_ON --->  menyalakan control valve 1\n"
//				"CV1_OFF ---> mematikan control valve 1\n"
//				"CV2_ON --->  menyalakan control valve 2\n"
//				"CV2_OFF ---> mematikan control valve 2\n"
//				"CV3_ON --->  menyalakan control valve 3\n"
//				"CV3_OFF ---> mematikan control valve 3\n "
//				"CV4_ON --->  menyalakan control valve 4\n"
//				"CV4_OFF ---> mematikan control valve 4\n "
//				"CV5_ON --->  menyalakan control valve 5\n"
//				"CV5_OFF ---> mematikan control valve 5\n"
//				"PUM1_ON ---> menyalakan pompa 1\n"
//				"PUM1_OFF ---> menyalakan pompa 1\n"
//				"PUM2_ON ---> menyalakan pompa 2\n"
//				"PUM2_OFF ---> menyalakan pompa 2\n "
//				"PUM3_ON ---> menyalakan pompa 3\n"
//				"PUM3_OFF ---> menyalakan pompa 3\n"
//				"Compressor_ON ---> menyalakan compressor\n"
//				"Compressor_OFF ---> mematikan compressor\n "
//				"Ozone_ON ---> menyalakan Ozone\n"
//				"Ozone_OFF ---> mematikan Ozone\n");
//	} else if (strcmp(argv[0], "EAB_ON") == 0) {
//		palClearPad(GPIOB, GPIO_PIN_1);
//		palClearPad(GPIOB, GPIO_PIN_10);
//		palClearPad(GPIOB, GPIO_PIN_11);
//		palClearPad(GPIOA, GPIO_PIN_8);
//		command_printf("EAB_ON\n");
//	}
//
//	else if (strcmp(argv[0], "EAB_OFF") == 0) {
//		palSetPad(GPIOB, GPIO_PIN_1);
//		palSetPad(GPIOB, GPIO_PIN_10);
//		palSetPad(GPIOB, GPIO_PIN_11);
//		palSetPad(GPIOA, GPIO_PIN_8);
//		command_printf("EAB_OFF\n");
//	}
//
//	else if (strcmp(argv[0], "CV1_ON") == 0) {
//		palClearPad(GPIOB, GPIO_PIN_0);
//		command_printf("Control Valve 1 On\n");
//	}
//
//	else if (strcmp(argv[0], "CV1_OFF") == 0) {
//		palSetPad(GPIOB, GPIO_PIN_0);
//		command_printf("Control Valve 1 Off\n");
//	}
//
//	else if (strcmp(argv[0], "CV2_ON") == 0) {
//		palClearPad(GPIOA, GPIO_PIN_7);
//		command_printf("Control Valve 2 On\n");
//	}
//
//	else if (strcmp(argv[0], "CV2_OFF") == 0) {
//		palSetPad(GPIOA, GPIO_PIN_7);
//		command_printf("Control Valve 2 Off\n");
//	}
//
//	else if (strcmp(argv[0], "CV3_ON") == 0) {
//		palClearPad(GPIOA, GPIO_PIN_6);
//		command_printf("Control Valve 3 On\n");
//	}
//
//	else if (strcmp(argv[0], "CV3_OFF") == 0) {
//		palSetPad(GPIOA, GPIO_PIN_6);
//		command_printf("Control Valve 3 Off\n");
//	}
//
//	else if (strcmp(argv[0], "CV4_ON") == 0) {
//		palClearPad(GPIOA, GPIO_PIN_5);
//		command_printf("Control Valve 4 On\n");
//	}
//
//	else if (strcmp(argv[0], "CV4_OFF") == 0) {
//		palSetPad(GPIOA, GPIO_PIN_5);
//		command_printf("Control Valve 4 Off\n");
//	}
//
//	else if (strcmp(argv[0], "CV5_ON") == 0) {
//		palClearPad(GPIOA, GPIO_PIN_4);
//		command_printf("Control Valve 5 On\n");
//	}
//
//	else if (strcmp(argv[0], "CV5_OFF") == 0) {
//		palSetPad(GPIOA, GPIO_PIN_4);
//		command_printf("Control Valve 5 Off\n");
//	}
//
//	else if (strcmp(argv[0], "Compressor_ON") == 0) {
//		palClearPad(GPIOB, GPIO_PIN_12);
//		palClearPad(GPIOB, GPIO_PIN_14);
//		command_printf("Compressor On\n");
//	}
//
//	else if (strcmp(argv[0], "Compressor_OFF") == 0) {
//		palSetPad(GPIOB, GPIO_PIN_14);
//		palSetPad(GPIOB, GPIO_PIN_12);
//		command_printf("Compressor Off\n");
//	}
//
//	else if (strcmp(argv[0], "Ozone_ON") == 0) {
//		palClearPad(GPIOB, GPIO_PIN_13);
//		command_printf("Ozone On\n");
//	}
//
//	else if (strcmp(argv[0], "Ozone_OFF") == 0) {
//		palSetPad(GPIOB, GPIO_PIN_13);
//		command_printf("Ozone Off\n");
//	}
//
//	else if (strcmp(argv[0], "PUMP1_ON") == 0) {
//		palClearPad(GPIOA, GPIO_PIN_3);
//		command_printf("PUMP1 On\n");
//	}
//
//	else if (strcmp(argv[0], "PUMP1_OFF") == 0) {
//		palSetPad(GPIOA, GPIO_PIN_3);
//		command_printf("PUMP1 Off\n");
//	}
//
//	else if (strcmp(argv[0], "PUMP2_ON") == 0) {
//		palClearPad(GPIOA, GPIO_PIN_2);
//		command_printf("PUMP2 On\n");
//	}
//
//	else if (strcmp(argv[0], "PUMP2_OFF") == 0) {
//		palSetPad(GPIOA, GPIO_PIN_2);
//		command_printf("PUMP2 Off\n");
//	}
//
//	else if (strcmp(argv[0], "PUMP3_ON") == 0) {
//		palClearPad(GPIOA, GPIO_PIN_1);
//		command_printf("PUMP3 On\n");
//	}
//
//	else if (strcmp(argv[0], "PUMP3_OFF") == 0) {
//		palSetPad(GPIOA, GPIO_PIN_1);
//		command_printf("PUMP3 Off\n");
//	}
//
//}
//
//void command_printf(const char *format, ...) {
//	va_list arg;
//	va_start(arg, format);
//	int len;
//	unsigned char print_buffer[255];
//	//char *buf;
//	len = vsnprintf(print_buffer, 254, format, arg);
//	va_end(arg);
//	if (len > 0) {
//		CDC_Transmit_FS((unsigned char*) print_buffer,
//				(len < 254) ? len + 1 : 255);
//	}
////	memset(print_buffer, 0, len);
//}
