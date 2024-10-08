#ifndef INC_MEMORY_H_
#define INC_MEMORY_H_

#include "inttypes.h"

#define MEMORY_BYTES 5
#define MEMORY_SHORTS 9
#define MEMORY_FLOATS 4
#define MEMORY_SIZE MEMORY_BYTES+2*MEMORY_SHORTS+4*MEMORY_FLOATS

//Change to reset memory on upload
#define MEMORY_HASH 125

enum MemoryType {
	MemByte,
	MemShort,
	MemFloat,
};

enum MemoryByte {
	MemHash,

	//pulses per revolution
	MemPPRTach,
	MemPPRSens1,
	MemPPRSens2,

	//if gps or sensor mode
	MemGPSMode,
};
enum MemoryShort{
	//min front wheel rpm
	MemMinRPM,

	//max rpm for ignoring higher
	MemMaxTach,
	MemMaxSens1,
	MemMaxSens2,

	//all out of 4096
	MemTachHighThresh,
	MemSens1Thresh,
	MemSens2Thresh,

	//for extra hysteresis if necessary
	MemTachLowThresh,

	MemFactoryPassword,
};
enum MemoryFloat{
	MemTireCirc,//inches

	//Min gps speed
	MemMinGPSSpeed,//mph

	MemSlipThresh,//percent

	MemSecondThresh,//mph
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
