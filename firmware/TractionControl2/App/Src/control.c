#include "control.h"
#include "main.h"
#include "memory.h"
#include "gps.h"
#include "tach.h"
#include "app.h"

static uint16_t tire_rpm = 0;
static uint16_t vehicle_rpm = 0;
static float slip = 0;

static uint16_t rpm_max(uint16_t a, uint16_t b) {
	if (a > b) {
		return a;
	}
	return b;
}

void Control_Update(void) {
	tire_rpm = rpm_max(Tach_RPM(TachEngine), Tach_RPM(TachSensor2));
#ifdef APP_GPS
	if (Memory_ReadByte(MemGPSMode)) {
		const uint8_t second = GPS_Fix() && (GPS_Speed() < Memory_ReadFloat(MemSecondThresh));
		HAL_GPIO_WritePin(DRV2_GPIO_Port, DRV2_Pin, second);
	}else{
#endif
		const uint8_t second = Memory_ReadFloat(MemTireCirc)*Tach_RPM(TachSensor1)*0.00094697 < Memory_ReadFloat(MemSecondThresh);
		HAL_GPIO_WritePin(DRV2_GPIO_Port, DRV2_Pin, second);
#ifdef APP_GPS
	}
#endif

#ifdef APP_GPS
	if (Memory_ReadByte(MemGPSMode)) {
		if (GPS_Fix()) {
			vehicle_rpm = GPS_RPM();
		}else{
			HAL_GPIO_WritePin(DRV1_GPIO_Port, DRV1_Pin, 0);
			return;
		}
	}else{
#endif
		vehicle_rpm = Tach_RPM(TachSensor1);
#ifdef APP_GPS
	}
#endif
//	const uint16_t min_rpm = Memory_ReadShort(MemMin);
//	vehicle_rpm = rpm_max(vehicle_rpm, min_rpm);
#ifdef APP_GPS
//	const uint16_t min_rpm = (Memory_ReadFloat(MemMinGPS)*63360.0)/(60.0*Memory_ReadFloat(MemTireCirc));
	if (!Memory_ReadByte(MemGPSMode)) {
		const uint16_t min_rpm = Memory_ReadShort(MemMinRPM);
		vehicle_rpm = rpm_max(vehicle_rpm, min_rpm);
	}
#endif
#ifndef APP_GPS
	const uint16_t min_rpm = Memory_ReadShort(MemMinRPM);
	vehicle_rpm = rpm_max(vehicle_rpm, min_rpm);
#endif
	if (vehicle_rpm < tire_rpm ) {
		slip = 100.0*(1.0f - (float)vehicle_rpm/(float)tire_rpm);
//		slip = 100.0*((float)tire_rpm-(float)vehicle_rpm)/(float)vehicle_rpm;
	} else {
		slip = 0;
	}

#ifdef APP_GPS
	if (GPS_Speed() < Memory_ReadFloat(MemMinGPSSpeed)) {
		HAL_GPIO_WritePin(DRV1_GPIO_Port, DRV1_Pin, 0);
		return;
	}
#endif

	HAL_GPIO_WritePin(DRV1_GPIO_Port, DRV1_Pin, slip > Memory_ReadFloat(MemSlipThresh));
}

float Control_Slip(void) {
	return slip;
}

float Control_Ratio(void) {
	if (vehicle_rpm == 0) {
		return 0;
	}
	return (float)tire_rpm/(float)vehicle_rpm;
}
