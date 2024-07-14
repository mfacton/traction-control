#include "tach.h"
#include "stm32g4xx_hal.h"
#include "memory.h"
#include "dac.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

extern COMP_HandleTypeDef hcomp1;
extern COMP_HandleTypeDef hcomp2;
extern COMP_HandleTypeDef hcomp3;
extern COMP_HandleTypeDef hcomp4;

static volatile uint32_t count[TACH_CHANNELS] = {0};

static volatile uint32_t countAccum[TACH_CHANNELS] = {0};
static volatile uint32_t passes[TACH_CHANNELS] = {0};

static volatile uint32_t checkpointAccum[TACH_CHANNELS] = {0};
static volatile uint32_t checkpointPasses[TACH_CHANNELS] = {0};

static uint16_t rpm[TACH_CHANNELS] = {0};

void Tach_Init(void) {
	HAL_COMP_Start(&hcomp1);
	HAL_COMP_Start(&hcomp2);
	HAL_COMP_Start(&hcomp3);
	//	HAL_COMP_Start(&hcomp4);

	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
}

void Tach_Update(void) {
	for (uint8_t i = 0; i < TACH_CHANNELS; i++) {
		if (checkpointPasses[i]) {
			rpm[i] = (float)TACH_COUNT_FREQ*60.0*(float)checkpointPasses[i]/((float)checkpointAccum[i]*(float)Memory_ReadByte(MemPPRTach+i));
			countAccum[i] = 0;
			checkpointAccum[i] = 0;
		}else if(passes[i]){
			rpm[i] = (float)TACH_COUNT_FREQ*60.0*(float)passes[i]/((float)countAccum[i]*(float)Memory_ReadByte(MemPPRTach+i));
			countAccum[i] = 0;
			checkpointAccum[i] = 0;
		}else {
			const uint16_t new_rpm = (float)TACH_COUNT_FREQ*60.0/((float)Memory_ReadByte(MemPPRTach+i)*(float)count[i]);
			if (new_rpm < rpm[i]) {
				rpm[i] = new_rpm;
			}
		}

		passes[i] = 0;
		checkpointPasses[i] = 0;
	}
}

static void tach_channel_handler(uint8_t chan) {
	if (count[chan] >= 60.0*(float)TACH_COUNT_FREQ/(Memory_ReadByte(MemPPRTach+chan)*Memory_ReadShort(MemMaxTach+chan))) {
		passes[chan]++;
		countAccum[chan]+=count[chan];
		if (passes[chan] % Memory_ReadByte(MemPPRTach+chan) == 0) {
			checkpointPasses[chan] = passes[chan];
			checkpointAccum[chan] = countAccum[chan];
		}
		count[chan] = 0;
	}
}

void Tach_CompHandler(COMP_HandleTypeDef* hcomp) {
	if (hcomp == &hcomp3) {
		tach_channel_handler(TachEngine);
	}else if (hcomp == &hcomp2) {
		tach_channel_handler(TachSensor1);
	}else{
		tach_channel_handler(TachSensor2);
	}
}

void Tach_CountHandler(void) {
	count[0]++;
	count[1]++;
	count[2]++;
}

uint16_t Tach_RPM(uint8_t chan) {
	return rpm[chan];
}
