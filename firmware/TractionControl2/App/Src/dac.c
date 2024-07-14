#include "dac.h"
#include "main.h"
#include "memory.h"

extern DAC_HandleTypeDef hdac1;
extern DAC_HandleTypeDef hdac3;

struct Dac dacs[DAC_CHANNELS] = {{.hdac = &hdac1, .chan = DAC_CHANNEL_1},
		{.hdac = &hdac1, .chan = DAC_CHANNEL_2},
		{.hdac = &hdac3, .chan = DAC_CHANNEL_1},
		{.hdac = &hdac3, .chan = DAC_CHANNEL_2}};

void Dac_Init(void) {
	Dac_Update();
}

void Dac_Update(void) {
	Dac_Set(DacSens2, Memory_ReadShort(MemSens2Thresh));
	Dac_Set(DacSens1, Memory_ReadShort(MemSens1Thresh));
	Dac_Set(DacTachHigh, Memory_ReadShort(MemTachHighThresh));
	Dac_Set(DacTachLow, Memory_ReadShort(MemTachLowThresh));
}

void Dac_Set(enum DacChannel dac, uint16_t value) {
	HAL_DAC_SetValue(dacs[dac].hdac, dacs[dac].chan, DAC_ALIGN_12B_R, value);
}
