#include "gps.h"
#include "stm32g4xx_hal.h"
#include "memory.h"
#include "stdio.h"
#include "string.h"
#include "main.h"
#include "display.h"

extern UART_HandleTypeDef GPS_HANDLE;

static uint8_t gps_rx_buf[GPS_BUFFER_SIZE] = {0};
static uint8_t gps_read_buf[GPS_BUFFER_SIZE] = {0};
static volatile uint8_t gps_flag = 0;

static uint8_t gps_fix = 0;
static float gps_speed = 0;//kph
static uint16_t gps_rpm = 0;

static uint8_t last_fix = 0;

void GPS_Init(void) {
	HAL_UART_Receive_DMA(&GPS_HANDLE, gps_rx_buf, GPS_BUFFER_SIZE);
}

void gps_read_data(void) {
	char* ptr = (char*)gps_read_buf;
	uint8_t cnt = 0;
	while (cnt < 2) {
		if (*ptr == '$') {
			cnt++;
		}
		ptr++;
	}
	if (*(ptr+2) != 'V') {
		gps_fix = 0;
		return;
	}
	cnt = 0;
	while (cnt < 7) {
		if (*ptr == ',') {
			cnt++;
		}
		ptr++;
	}
	if (*(ptr+1) == ',') {
		gps_fix = 0;
		return;
	}
	char* ptr_end = ptr;
	while (*ptr_end != ',') {
		ptr_end++;
	}
	*ptr_end = '\0';
	sscanf(ptr, "%f", &gps_speed);
	float speed_temp = gps_speed;
	//convert mph to kph
	const float min_spd = Memory_ReadFloat(MemMinSpeed)*1.60934;
	if (speed_temp < min_spd) {
		speed_temp = min_spd;
	}
	gps_rpm = (uint16_t)((float)(1e6/60.0)*speed_temp/(float)Memory_ReadFloat(MemTireCirc));
	gps_fix = 1;
}

void GPS_Update(void) {
	if (gps_flag) {
		gps_flag = 0;
		gps_read_data();
		if (gps_fix != last_fix) {
			last_fix = gps_fix;
			Display_UpdateGPS();
		}
	}
}

void GPS_Handler(void) {
	memcpy(gps_read_buf, gps_rx_buf, GPS_BUFFER_SIZE);
	gps_flag = 1;
	HAL_UART_Receive_DMA(&GPS_HANDLE, gps_rx_buf, GPS_BUFFER_SIZE);
}

uint16_t GPS_RPM(void) {
	return gps_rpm;
}

float GPS_Speed(void) {
	return gps_speed;
}

uint8_t GPS_Fix(void) {
	return gps_fix;
}
