#include "control.h"
#include "main.h"
#include "tach.h"
#include "gps.h"
#include "input.h"
#include "display.h"
#include "memory.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

static uint16_t tire_rpm = 0;
static uint16_t vehicle_rpm = 0;
static float slip = 0;

static uint16_t rpm_max(uint16_t a, uint16_t b) {
	if (a > b) {
		return a;
	}
	return b;
}

static void control_update(void) {
	tire_rpm = rpm_max(Tach_RPM(TachEngine), Tach_RPM(TachSensor2));
	HAL_GPIO_WritePin(DRV2_GPIO_Port, DRV2_Pin, tire_rpm < Memory_ReadShort(MemRPMThresh));

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
		slip = 100.0 - (float)vehicle_rpm * 100.0 / (float)tire_rpm;
	} else {
		slip = 0;
	}

	HAL_GPIO_WritePin(DRV1_GPIO_Port, DRV1_Pin, slip > Memory_ReadFloat(MemSlipThresh));
}

void Control_Init(void) {
//	Memory_Init();
//	Tach_Init();
	GPS_Init();
	Display_Init();

//	HAL_TIM_Base_Start_IT(&htim1);
//	HAL_TIM_Base_Start_IT(&htim2);
//	HAL_TIM_Base_Start_IT(&htim3);
}

void Control_Loop(void) {
	GPS_Update();
//	control_update();
	Display_Update();
	HAL_Delay(50);
}

float Control_Slip(void) {
	return slip;
}

float Control_Ratio(void) {
	if (tire_rpm == 0) {
		return 0;
	}
	return (float)vehicle_rpm/(float)tire_rpm;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim1) {
//		Input_Handler();//50hz
	}else if (htim == &htim2) {
//		Tach_CountHandler();//50khz
	}else {
//		Tach_UpdateHandler();//50hz
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t pin) {
	if (pin == SIG1_Pin) {
//		Tach_EventHandler(1);
	}else{
//		Tach_EventHandler(2);
	}
}
