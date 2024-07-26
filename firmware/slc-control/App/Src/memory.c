#include "memory.h"
#include "stm32g4xx_hal.h"
#include "eeprom.h"
#include "stdlib.h"

static uint8_t mem_buf[MEMORY_SIZE];

void Memory_Init(void) {
	Eeprom_Read(0, mem_buf, MEMORY_SIZE);
	HAL_Delay(2);
	if (Memory_ReadByte(MemHash) != MEMORY_HASH) {
		Memory_WriteByte(MemHash, MEMORY_HASH);
		Memory_Reset();
	}
}

void Memory_Reset(void) {
	//Bytes
	Memory_WriteByte(MemPPRTach, 10);
	Memory_WriteByte(MemPPRSens1, 10);
	Memory_WriteByte(MemPPRSens2, 10);

	Memory_WriteByte(MemGPSMode, 0);
	//Shorts
	Memory_WriteShort(MemMinRPM, 20);

	Memory_WriteShort(MemMaxSens2, 10000);
	Memory_WriteShort(MemMaxSens1, 10000);
	Memory_WriteShort(MemMaxTach, 10000);

	Memory_WriteShort(MemSens2Thresh, 2048);
	Memory_WriteShort(MemSens1Thresh, 2048);
	Memory_WriteShort(MemTachHighThresh, 2048);
	Memory_WriteShort(MemTachLowThresh, 2048);

	Memory_WriteShort(MemFactoryPassword, 0);//reset to 0
	//Floats
	Memory_WriteFloat(MemTireCirc, 53.4); //53.4 inches

	Memory_WriteFloat(MemMinSpeed, 2.0); //mph

	Memory_WriteFloat(MemSlipThresh, 10.0); //10.0%

	Memory_WriteFloat(MemSecondThresh, 60.0); //mph
}

uint8_t Memory_ReadByte(uint8_t loc) {
	return mem_buf[loc];
}
uint16_t Memory_ReadShort(uint8_t loc) {
	const uint16_t pos = MEMORY_BYTES + 2 * loc;
	return *((uint16_t*) (mem_buf + pos));
}
float Memory_ReadFloat(uint8_t loc) {
	const uint16_t pos = MEMORY_BYTES + 2 * MEMORY_SHORTS + 4 * loc;
	return *((float*) (mem_buf + pos));
}

void Memory_WriteByte(uint8_t loc, uint8_t val) {
	mem_buf[loc] = val;
	Eeprom_Write(loc, &val, 1);
	HAL_Delay(2);
}
void Memory_WriteShort(uint8_t loc, uint16_t val) {
	const uint16_t pos = MEMORY_BYTES + 2 * loc;
	uint16_t *ptr = (uint16_t*) (mem_buf + pos);
	ptr[0] = val;
	Eeprom_Write(pos, (uint8_t*) &val, 2);
	HAL_Delay(2);
}
void Memory_WriteFloat(uint8_t loc, float val) {
	const uint16_t pos = MEMORY_BYTES + 2 * MEMORY_SHORTS + 4 * loc;
	float *ptr = (float*) (mem_buf + pos);
	ptr[0] = val;
	Eeprom_Write(pos, (uint8_t*) &val, 4);
	HAL_Delay(2);
}
