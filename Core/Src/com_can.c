/*
 * com_can.c
 *
 *  Created on: Apr 27, 2022
 *      Author: rangga
 */
#include "stm32f1xx.h"
#include "com_can.h"
#include "string.h"
#include "fungsi.h"
#include "crc.h"

#define CAN_PACKET_SETMODE 35
#define CAN_PACKET_PROCESS_SHORT_BUFFER 8
#define CAN_PACKET_FILL_RX_BUFFER 5
#define CAN_PACKET_FILL_RX_BUFFER_LONG 6
#define CAN_PACKET_PROCESS_RX_BUFFER 7
#define RX_BUFFER_SIZE 64
#define CAN_PACKET_SET_DUTY_GEN 36
#define CAN_PACKET_ERRORMASSAGE 37
#define CAN_PACKET_FILL_RX_BUFFER_F1 40

extern CAN_HandleTypeDef hcan;
extern uint32_t selfID;

CAN_RxHeaderTypeDef RxHeader;
CAN_TxHeaderTypeDef TxHeader;

uint8_t TxData[8];
uint8_t RxData[8];

uint32_t TxMailbox, ind;

static uint8_t rx_buffer[RX_BUFFER_SIZE];
unsigned int rxbuf_ind;

uint8_t crc_low;
uint8_t crc_high;
uint8_t commands_send;

unsigned int rxbuf_len;
extern unsigned int rxbuf_ind;

int canSetMode;
extern uint32_t mode;

//static unsigned int rx_buffer_last_id;

// Callback untuk penerimaan data can bus
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {

	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, RxData);

	if (RxHeader.IDE == CAN_ID_EXT) {
		uint8_t id = RxHeader.ExtId & 0xFF;
		uint8_t cmd = RxHeader.ExtId >> 8;

		if (id == 255 || id == selfID) {
			switch (cmd) {
			case CAN_PACKET_SETMODE:
				canSetMode = -1;
				ind = 0;
				mode = buffer_get_int32(RxData, &ind);
				//mode = canSetMode;
				//setMode(mode);
				break;
			case CAN_PACKET_FILL_RX_BUFFER:
				memcpy(rx_buffer + RxData[0], RxData + 1, RxHeader.DLC - 1);
				break;

			case CAN_PACKET_FILL_RX_BUFFER_LONG:
				rxbuf_ind = (unsigned int) RxData[0] << 8;
				rxbuf_ind |= RxData[1];
				if (rxbuf_ind < RX_BUFFER_SIZE) {
					memcpy(rx_buffer + rxbuf_ind, RxData + 2, RxHeader.DLC - 2);
				}
				break;

			}

		}
	}
	//}

}

void buffer_append_int32(uint8_t *buffer, int32_t number, int32_t *index) {
	buffer[(*index)++] = number >> 24;
	buffer[(*index)++] = number >> 16;
	buffer[(*index)++] = number >> 8;
	buffer[(*index)++] = number;
}

int32_t buffer_get_int32(const uint8_t *buffer, int32_t *index) {
	int32_t res = ((uint32_t) buffer[*index]) << 24
			| ((uint32_t) buffer[*index + 1]) << 16
			| ((uint32_t) buffer[*index + 2]) << 8
			| ((uint32_t) buffer[*index + 3]);
	*index += 4;
	return res;
}

void comm_can_transmit_eid(uint32_t id, const uint8_t *data, uint8_t len) {
	if (len > 8) {
		len = 8;
	}

	TxHeader.IDE = CAN_ID_EXT;
	TxHeader.ExtId = id;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = len;
	memcpy(TxData, data, len);
	HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
}

void comm_can_set_duty(uint8_t controller_id, float duty) {
	int32_t send_index = 0;
	uint8_t buffer[4];
	buffer_append_int32(buffer, (int32_t) (duty * 100.0), &send_index);
	comm_can_transmit_eid(
			controller_id | ((uint32_t) CAN_PACKET_SET_DUTY_GEN << 8), buffer,
			send_index);
}

void comm_can_db_signal(uint8_t controller_id, int command) {
	int32_t send_index = 0;
	uint8_t buffer[4];
	buffer_append_int32(buffer, (int32_t) command, &send_index);
	comm_can_transmit_eid(controller_id | ((uint32_t) 35 << 8), buffer,
			send_index);
}

void comm_can_error_message(uint8_t controller_id, int command) {
	int32_t send_index = 0;
	uint8_t buffer[4];
	buffer_append_int32(buffer, (int32_t) command, &send_index);
	comm_can_transmit_eid(
			controller_id | ((uint32_t) CAN_PACKET_ERRORMASSAGE << 8), buffer,
			send_index);
}

void comm_can_send_buffer(uint8_t controller_id, uint8_t *data,
		unsigned int len, uint8_t send) {
	uint8_t send_buffer[8];

	if (len <= 6) {
		uint32_t ind = 0;
		send_buffer[ind++] = controller_id;
		send_buffer[ind++] = send;	     // untuk memilih commands pada receiver
		memcpy(send_buffer + ind, data, len);
		ind += len;
		comm_can_transmit_eid(
				controller_id
						| ((uint32_t) CAN_PACKET_PROCESS_SHORT_BUFFER << 8),
				send_buffer, ind);
	} else {
		// kayaknya ini yang terpakai untuk pengiriman data can bus

		unsigned int end_a = 0;
		for (unsigned int i = 0; i < len; i += 7) {
			if (i > 255) {
				break;
			}

			end_a = i + 7;

			uint8_t send_len = 7;
			send_buffer[0] = i;

			if ((i + 7) <= len) {
				memcpy(send_buffer + 1, data + i, send_len);
			} else {
				send_len = len - i;
				memcpy(send_buffer + 1, data + i, send_len);
			}

			comm_can_transmit_eid(
					controller_id
							| ((uint32_t) CAN_PACKET_FILL_RX_BUFFER_F1 << 8),
					send_buffer, send_len + 1);
		}
		//------------------------------------------------

	}
}
