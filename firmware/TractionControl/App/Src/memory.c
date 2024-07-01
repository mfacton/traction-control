#include "memory.h"
#include "stm32f1xx_hal.h"
#include "eeprom.h"
#include "stdlib.h"

static uint8_t mem_buf[MEMORY_SIZE];

void Memory_Init(void) {
	Eeprom_Read(0, mem_buf, MEMORY_SIZE);
}

void Memory_Reset(void) {
	Memory_WriteByte(MemPPR1, 10);
	Memory_WriteByte(MemPPR2, 10);
	Memory_WriteByte(MemPPR3, 10);
	Memory_WriteByte(MemGPSMode, 0);

	Memory_WriteShort(MemMinRPM, 20);
	Memory_WriteShort(MemRPMThresh, 2000);
	Memory_WriteShort(MemMaxRPM1, 3000);
	Memory_WriteShort(MemMaxRPM2, 8000);
	Memory_WriteShort(MemMaxRPM3, 8000);
	Memory_WriteShort(MemTachThresh, 1000);//3.3V/4096

	Memory_WriteFloat(MemTireCirc, 85.0);//53.4 inches
	Memory_WriteFloat(MemMinSpeed, 2);//2 mph
	Memory_WriteFloat(MemSlipThresh, 10.0);//10.0%
}

uint8_t Memory_ReadByte(uint8_t loc) {
	return mem_buf[loc];
}
uint16_t Memory_ReadShort(uint8_t loc) {
	const uint16_t pos = MEMORY_BYTES+2*loc;
	return *((uint16_t*)(mem_buf+pos));
}
float Memory_ReadFloat(uint8_t loc) {
	const uint16_t pos = MEMORY_BYTES+2*MEMORY_SHORTS+4*loc;
	return *((float*)(mem_buf+pos));
}

void Memory_WriteByte(uint8_t loc, uint8_t val) {
	mem_buf[loc] = val;
	Eeprom_Write(loc, &val, 1);
	HAL_Delay(2);
}
void Memory_WriteShort(uint8_t loc, uint16_t val) {
	const uint16_t pos = MEMORY_BYTES+2*loc;
	uint16_t* ptr = (uint16_t*)(mem_buf+pos);
	ptr[0] = val;
	Eeprom_Write(pos, (uint8_t*)&val, 2);
	HAL_Delay(2);
}
void Memory_WriteFloat(uint8_t loc, float val) {
	const uint16_t pos = MEMORY_BYTES+2*MEMORY_SHORTS+4*loc;
	float* ptr = (float*)(mem_buf+pos);
	ptr[0] = val;
	Eeprom_Write(pos, (uint8_t*)&val, 4);
	HAL_Delay(2);
}
