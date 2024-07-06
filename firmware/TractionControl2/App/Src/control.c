#include "control.h"
#include "main.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

void Control_Init(void) {
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
}

void Control_Loop(void) {

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim3) {
		Tach_CountHandler();//50KHz
	}else if (htim == &htim2) {
		Tach_UpdateHandler();//50Hz
	}else {
		Input_Handler();//50Hz
	}
}
