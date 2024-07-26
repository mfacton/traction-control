#ifndef INC_TACH_H_
#define INC_TACH_H_

#include "stm32g4xx_hal.h"

#define TACH_CHANNELS 3
#define TACH_UPDATE_FREQ 50
#define TACH_COUNT_FREQ 50000

enum TachChannels {
	TachEngine,
	TachSensor1,
	TachSensor2,
};

void Tach_Init(void);
void Tach_Update(void);

void Tach_CountHandler(void);
void Tach_CompHandler(COMP_HandleTypeDef* hcomp);

uint16_t Tach_RPM(uint8_t chan);

#endif
