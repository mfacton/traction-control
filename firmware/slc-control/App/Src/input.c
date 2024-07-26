#include "input.h"
#include "main.h"

extern TIM_HandleTypeDef INPUT_HANDLE;

static GPIO_TypeDef *ports[INPUT_INPUTS] = { BTN1_GPIO_Port, BTN2_GPIO_Port,
		BTN3_GPIO_Port, BTN4_GPIO_Port };
static uint16_t pins[INPUT_INPUTS] = { BTN1_Pin, BTN2_Pin, BTN3_Pin, BTN4_Pin };

static uint8_t lastState[INPUT_INPUTS];
static uint8_t pressed[INPUT_INPUTS];

void Input_Init(void) {
	HAL_TIM_Base_Start_IT(&INPUT_HANDLE);
}

void Input_Handler(void) {
	for (uint8_t i = 0; i < INPUT_INPUTS; i++) {
		uint8_t state = !HAL_GPIO_ReadPin(ports[i], pins[i]);
		if (state && !lastState[i]) {
			pressed[i] = 1;
		}
		lastState[i] = state;
	}
}

uint8_t Input_Pressed(uint8_t input) {
	const uint8_t val = pressed[input];
	pressed[input] = 0;
	return val;
}
