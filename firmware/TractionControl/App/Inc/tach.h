#ifndef INC_TACH_H_
#define INC_TACH_H_

#include "inttypes.h"

#define TACH_CHANNELS 3
#define TACH_UPDATE_FREQ 50
#define TACH_COUNT_FREQ 50000

enum TachChannels {
	TachEngine,
	TachSensor1,
	TachSensor2,
};

void Tach_Init(void);
void Tach_UpdateHandler(void);

void Tach_CountHandler(void);
void Tach_EventHandler(uint8_t chan);

uint16_t Tach_RPM(uint8_t chan);

#endif
