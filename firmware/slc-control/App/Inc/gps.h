#ifndef INC_GPS_H_
#define INC_GPS_H_

#include "stdint.h"

#define GPS_HANDLE huart3
#define GPS_DMA_HANDLE hdma_usart3_rx

#define GPS_BUFFER_SIZE 255

void GPS_Init(void);
void GPS_Update(void);

void GPS_Handler(void);

uint16_t GPS_RPM(void);
float GPS_Speed(void);//return mph
uint8_t GPS_Fix(void);

#endif
