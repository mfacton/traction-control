#include "control.h"
#include "main.h"
#include "memory.h"
#include "gps.h"
#include "tach.h"

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

	if (Memory_ReadByte(MemGPSMode)) {
		const uint8_t second = GPS_Fix() && (GPS_Speed() < Memory_ReadFloat(MemSecondThresh));
		HAL_GPIO_WritePin(DRV2_GPIO_Port, DRV2_Pin, second);
	}else{
		const uint8_t second = Memory_ReadFloat(MemTireCirc)*Tach_RPM(TachSensor1)*0.00094697 < Memory_ReadFloat(MemSecondThresh);
		HAL_GPIO_WritePin(DRV2_GPIO_Port, DRV2_Pin, second);
	}

	if (Memory_ReadByte(MemGPSMode)) {
		if (GPS_Fix()) {
			vehicle_rpm = GPS_RPM();
		}else{
			HAL_GPIO_WritePin(DRV1_GPIO_Port, DRV1_Pin, 0);
			return;
		}
	}else{
		vehicle_rpm = Tach_RPM(TachSensor1);
	}
	vehicle_rpm = rpm_max(vehicle_rpm, Memory_ReadShort(MemMinRPM));
	if (vehicle_rpm < tire_rpm) {
		slip = 100.0*((float)tire_rpm-(float)vehicle_rpm/(float)vehicle_rpm);
	} else {
		slip = 0;
	}

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
