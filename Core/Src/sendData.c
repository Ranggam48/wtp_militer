/*
 * sendData.c
 *
 *  Created on: Oct 13, 2022
 *      Author: rangg
 */
#include "sendData.h"
#include "fungsi.h"

extern uint32_t timerEAB;
extern float currentEAB;
extern float voltEAB;
extern uint8_t indicator1, indicator2;
extern uint32_t mode;

uint8_t bufferCanTx[32];

void manageData(void) {

	// data timer proses EAB
	bufferCanTx[0] = timerEAB & 0xff;
	bufferCanTx[1] = timerEAB >> 8;

	// data arus eab karena float dikali 100 dulu
	bufferCanTx[2] = (uint32_t) (currentEAB * 100) & 0xff;
	bufferCanTx[3] = (uint32_t) (currentEAB * 100) >> 8;

	// data teganagn eab karena float dikali 100 dulu
	bufferCanTx[4] = (uint32_t) (voltEAB * 100) & 0xff;
	bufferCanTx[5] = (uint32_t) (voltEAB * 100) >> 8;

	//data indicator
//	bufferCanTx[6] = indicator1;
//	bufferCanTx[7] = indicator2;

	bufferCanTx[15] = (uint8_t) mode;

}
