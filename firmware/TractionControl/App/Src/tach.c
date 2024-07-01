#include "tach.h"
#include "stm32f1xx_hal.h"
#include "memory.h"

static uint32_t analog = 0;
static uint8_t lastState = 0;

uint32_t adc_count = 0;

static volatile uint32_t count[TACH_CHANNELS] = {0};

static volatile uint32_t countAccum[TACH_CHANNELS] = {0};
static volatile uint32_t passes[TACH_CHANNELS] = {0};

static volatile uint32_t checkpointAccum[TACH_CHANNELS] = {0};
static volatile uint32_t checkpointPasses[TACH_CHANNELS] = {0};


static uint16_t rpm[TACH_CHANNELS] = {0};

extern ADC_HandleTypeDef hadc1;

void Tach_Init(void) {
	HAL_ADC_Start_DMA(&hadc1, &analog, 1);
}

void Tach_UpdateHandler(void) {
	for (uint8_t i = 0; i < TACH_CHANNELS; i++) {
		if (checkpointPasses[i]) {
			rpm[i] = (float)TACH_COUNT_FREQ*60.0*(float)checkpointPasses[i]/((float)checkpointAccum[i]*(float)Memory_ReadByte(MemPPR1+i));
			countAccum[i] = 0;
			checkpointAccum[i] = 0;
		}else if(passes[i]){
			rpm[i] = (float)TACH_COUNT_FREQ*60.0*(float)passes[i]/((float)countAccum[i]*(float)Memory_ReadByte(MemPPR1+i));
			countAccum[i] = 0;
			checkpointAccum[i] = 0;
		}else {
			const uint16_t new_rpm = (float)TACH_COUNT_FREQ*60.0/((float)Memory_ReadByte(MemPPR1+i)*(float)count[i]);
			if (new_rpm < rpm[i]) {
				rpm[i] = new_rpm;
			}
		}

		passes[i] = 0;
		checkpointPasses[i] = 0;
	}
}

void Tach_CountHandler(void) {
	count[0]++;
	count[1]++;
	count[2]++;
}

void Tach_EventHandler(uint8_t chan) {
	if (count[chan] >= 60.0*(float)TACH_COUNT_FREQ/(Memory_ReadByte(MemPPR1+chan)*Memory_ReadShort(MemMaxRPM1+chan))) {
		passes[chan]++;
		countAccum[chan]+=count[chan];
		if (passes[chan] % Memory_ReadByte(MemPPR1+chan) == 0) {
			checkpointPasses[chan] = passes[chan];
			checkpointAccum[chan] = countAccum[chan];
		}
		count[chan] = 0;
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	UNUSED(hadc);
	const uint8_t state = analog > Memory_ReadShort(MemTachThresh);
	if (state && !lastState) {
		Tach_EventHandler(0);
	}
	lastState = state;
	HAL_ADC_Start_DMA(&hadc1, &analog, 1);
	adc_count++;
}

uint16_t Tach_RPM(uint8_t chan) {
	return rpm[chan];
}
