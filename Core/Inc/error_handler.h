/*
 * error_handler.h
 *
 *  Created on: Sep 27, 2022
 *      Author: rangg
 */
#include"stm32f1xx.h"

#ifndef INC_ERROR_HANDLER_H_
#define INC_ERROR_HANDLER_H_

void errorCheckEAB(float current, float volt);
void errorCheckTank(uint8_t level);
void errorCheckOksigen(float oksigen);
void errorCheckOzone(uint8_t ozone);

#endif /* INC_ERROR_HANDLER_H_ */
