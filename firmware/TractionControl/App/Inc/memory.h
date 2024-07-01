#ifndef INC_MEMORY_H_
#define INC_MEMORY_H_

#include "inttypes.h"

#define MEMORY_BYTES 4
#define MEMORY_SHORTS 6
#define MEMORY_FLOATS 3
#define MEMORY_SIZE MEMORY_BYTES+2*MEMORY_SHORTS+4*MEMORY_FLOATS

enum MemoryType {
	MemByte,
	MemShort,
	MemFloat,
};

enum MemoryByte {
	MemPPR1,
	MemPPR2,
	MemPPR3,
	MemGPSMode,
};
enum MemoryShort{
	MemMinRPM,
	MemRPMThresh,
	MemMaxRPM1,
	MemMaxRPM2,
	MemMaxRPM3,
	MemTachThresh,
};
enum MemoryFloat{
	MemTireCirc,
	MemMinSpeed,
	MemSlipThresh,
};

void Memory_Init(void);
void Memory_Reset(void);

uint8_t Memory_ReadByte(uint8_t loc);
uint16_t Memory_ReadShort(uint8_t loc);
float Memory_ReadFloat(uint8_t loc);

void Memory_WriteByte(uint8_t loc, uint8_t val);
void Memory_WriteShort(uint8_t loc, uint16_t val);
void Memory_WriteFloat(uint8_t loc, float val);

#endif
