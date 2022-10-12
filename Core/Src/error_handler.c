/*
 * error_handler.c
 *
 *  Created on: Sep 27, 2022
 *      Author: rangg
 */

#include "error_handler.h"
#include "com_can.h"

void errorCheckEAB(float current, float volt) {
	if (current < 10) {
		comm_can_error_message(7, 0x00);
	} else if (volt < 12) {
		comm_can_error_message(7, 0x01);
	}
}

void errorCheckTank(uint8_t level) {
	if (level == 1) {
		comm_can_error_message(7, 0x02);
	}
}

void errorCheckOksigen(float oksigen) {
	if (oksigen < 20) {
		comm_can_error_message(7, 0x03);
	}
}

void errorCheckOzone(uint8_t ozone) {
	if (ozone < 1000) {
		comm_can_error_message(7, 0x04);
	}
}
