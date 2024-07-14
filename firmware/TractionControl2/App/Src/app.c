#include "app.h"
#include "main.h"
#include "memory.h"
#include "input.h"
#include "dac.h"
#include "tach.h"
#include "gps.h"
#include "display.h"
#include "oled.h"
#include "control.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

void App_Init(void) {
	Oled_Init();
	Memory_Init();
	Dac_Init();
	Tach_Init();
	GPS_Init();
	Display_Init();
	Input_Init();
}

void App_Loop(void) {
	GPS_Update();
	Control_Update();
	Dac_Update();
	Display_Update();
	HAL_Delay(50);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim3) {
		Tach_CountHandler();//50KHz
	}else if (htim == &htim2) {
		Tach_Update();//50Hz
	}else {
		Input_Handler();//50Hz
	}
}

void HAL_COMP_TriggerCallback(COMP_HandleTypeDef* hcomp) {
	Tach_CompHandler(hcomp);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	UNUSED(huart);
	GPS_Handler();
}
