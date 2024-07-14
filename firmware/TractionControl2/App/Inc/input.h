#ifndef INC_INPUT_H_
#define INC_INPUT_H_

#define INPUT_INPUTS 4

#include "inttypes.h"

enum Buttons {
	ButtonUp,
	ButtonDown,
	ButtonEnter,
	ButtonBack,
};

void Input_Init(void);

void Input_Handler(void);

uint8_t Input_Pressed(uint8_t input);

#endif
