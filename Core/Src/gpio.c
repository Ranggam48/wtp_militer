/*
 * gpio.c
 *
 *  Created on: May 17, 2022
 *      Author: rangga
 */
#include <gpio.h>
#include "stm32f1xx.h"

extern uint16_t indicator;
extern uint8_t indicator1, indicator2;
extern uint8_t bufferCanTx[32];

void gpio_init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB,
	GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |
	GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA,
	GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
	GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	/*Configure GPIO pin : PB11 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_10 | GPIO_PIN_11
			| GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : PA8 */
	GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4
			| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PA8 */
//	GPIO_InitStruct.Pin = GPIO_PIN_13;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	/*Configure GPIO pin : PB5 */
	GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PB5 */
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//	/*Configure GPIO pin : PB5 */
//	GPIO_InitStruct.Pin = GPIO_PIN_5;
//	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//	/* EXTI interrupt init*/
//	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
//	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

//void EAB(uint8_t value) {
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, value);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, value);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, value);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, value);
//}
//
//void Control_Valve_1(uint8_t value) {
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, value);
//}
//
//void Control_Valve_2(uint8_t value) {
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, value);
//}
//
//void Control_Valve_3(uint8_t value) {
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, value);
//}
//
//void Control_Valve_4(uint8_t value) {
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, value);
//}
//
//void Control_Valve_5(uint8_t value) {
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, value);
//}
//
//void Compressor(uint8_t value) {
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, value);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, value);
//}
//
//void Ozone(uint8_t value) {
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, value);
//}
//
//void Pump_1(uint8_t value) {
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, value);
//}
//
//void Pump_2(uint8_t value) {
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, value);
//}
//
//void Pump_3(uint8_t value) {
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, value);
//}

void EAB(uint8_t value) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, value);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, value);
	if (value == 1) {
		bufferCanTx[12] = 0;
	} else {
		bufferCanTx[12] = 1;
	}

}

void Control_Valve_1(uint8_t value) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, value);
	if (value == 1) {
		bufferCanTx[11] = 0;
	} else {
		bufferCanTx[11] = 1;
	}

}

void Control_Valve_2(uint8_t value) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, value);
	if (value == 1) {
		bufferCanTx[10] = 0;
	} else {
		bufferCanTx[10] = 1;
	}
}

void Control_Valve_3(uint8_t value) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, value);
	if (value == 1) {
		bufferCanTx[9] = 0;
	} else {
		bufferCanTx[9] = 1;
	}

}

void Control_Valve_4(uint8_t value) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, value);
//	if (value == 1) {
//		bufferCanTx[8] = 0;
//	} else {
//		bufferCanTx[8] = 1;
//	}
}

void Control_Valve_5(uint8_t value) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, value);
//	if (value == 1) {
//		bufferCanTx[8] = 0;
//	} else {
//		bufferCanTx[8] = 1;
//	}
}

void Compressor(uint8_t value) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, value);
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, value);
	if (value == 1) {
		bufferCanTx[13] = 0;
	} else {
		bufferCanTx[13] = 1;
	}

}

void Ozone(uint8_t value) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, value);
	if (value == 1) {
		bufferCanTx[14] = 0;
	} else {
		bufferCanTx[14] = 1;
	}
}

void Pump_1(uint8_t value) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, value);
	if (value == 1) {
		bufferCanTx[8] = 0;
	} else {
		bufferCanTx[8] = 1;
	}
}

void Pump_2(uint8_t value) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, value);

	if (value == 1) {
		bufferCanTx[7] = 0;
	} else {
		bufferCanTx[7] = 1;
	}
}

void Pump_3(uint8_t value) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, value);
	if (value == 1) {
		bufferCanTx[6] = 0;
	} else {
		bufferCanTx[6] = 1;
	}
}
