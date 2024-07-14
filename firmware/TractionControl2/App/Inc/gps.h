#ifndef INC_GPS_H_
#define INC_GPS_H_

#include "stdint.h"

#define GPS_HANDLE huart1
#define GPS_BUFFER_SIZE 512

void GPS_Init(void);
void GPS_Update(void);

void GPS_Handler(void);

uint16_t GPS_RPM(void);
float GPS_Speed(void);
uint8_t GPS_Fix(void);

#endif
