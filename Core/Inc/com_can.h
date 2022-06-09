/*
 * com_can.h
 *
 *  Created on: Apr 27, 2022
 *      Author: rangg
 */

#ifndef INC_COM_CAN_H_
#define INC_COM_CAN_H_

#endif /* INC_COM_CAN_H_ */

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
int32_t buffer_get_int32(const uint8_t *buffer, int32_t *index);
void buffer_append_int32(uint8_t *buffer, int32_t number, int32_t *index);
void comm_can_transmit_eid(uint32_t id, const uint8_t *data, uint8_t len);
void comm_can_set_duty(uint8_t controller_id, float duty);
void comm_can_db_signal(uint8_t controller_id, int command);
void comm_can_send_buffer(uint8_t controller_id, uint8_t *data,
		unsigned int len, uint8_t send);
