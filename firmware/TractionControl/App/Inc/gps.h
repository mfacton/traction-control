#ifndef INC_GPS_H_
#define INC_GPS_H_

#include "stdint.h"

#define GPS_BUFFER 256

void GPS_Init(void);
void GPS_Update(void);

uint16_t GPS_RPM(void);
float GPS_Speed(void);
uint8_t GPS_Fix(void);

#endif
