/*
 * gpio.h
 *
 *  Created on: May 17, 2022
 *      Author: rangga
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"

void gpio_init(void);
void EAB(uint8_t value);
void Control_Valve_1(uint8_t value);
void Control_Valve_2(uint8_t value);
void Control_Valve_3(uint8_t value);
void Control_Valve_4(uint8_t value);
void Control_Valve_5(uint8_t value);
void Compressor(uint8_t value);
void Ozone(uint8_t value);
void Pump_1(uint8_t value);
void Pump_2(uint8_t value);
void Pump_3(uint8_t value);


#endif /* INC_GPIO_H_ */
