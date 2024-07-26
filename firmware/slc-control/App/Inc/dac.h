#ifndef INC_DAC_H_
#define INC_DAC_H_

#include "stm32g4xx_hal.h"

#define DAC_CHANNELS 4

struct Dac {
	DAC_HandleTypeDef *hdac;
	uint32_t chan;
};

enum DacChannel {
	DacSens2,
	DacSens1,
	DacTachHigh,
	DacTachLow,
};

void Dac_Init(void);
void Dac_Update(void);

void Dac_Set(enum DacChannel dac, uint16_t value);

#endif
