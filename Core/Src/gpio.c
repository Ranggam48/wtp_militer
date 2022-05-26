/*
 * gpio.c
 *
 *  Created on: May 17, 2022
 *      Author: rangga
 */
#include <gpio.h>
#include "stm32f1xx.h"




void gpio_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |
		  GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
		  GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8, GPIO_PIN_SET);

  /*Configure GPIO pin : PB11 */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |
		  	  	  	  	  GPIO_PIN_13 | GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |
		  	  	  	  	  GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}



void EAB(uint8_t value)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, value);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, value);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, value);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, value);
}

void Control_Valve_1(uint8_t value)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, value);
}

void Control_Valve_2(uint8_t value)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, value);
}

void Control_Valve_3(uint8_t value)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, value);
}

void Control_Valve_4(uint8_t value)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, value);
}

void Control_Valve_5(uint8_t value)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, value);
}

void Compressor(uint8_t value)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, value);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, value);
}

void Ozone(uint8_t value)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, value);
}

void Pump_1(uint8_t value)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, value);
}

void Pump_2(uint8_t value)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, value);
}

void Pump_3(uint8_t value)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, value);
}

