#include "main.h"
#include "control.h"
#include "ssd1306.h"
#include "stdio.h"
#include "i2c_eeprom.h"

#define SPOKES_DEFAULT 20
#define TAC_MIN_TIM 20
#define UPDATE_FREQ 50

static volatile uint32_t counter = 0;
static volatile uint32_t last_counter = 0;

static uint16_t rpm = 0;
//stores until every refresh which is 50hz
//256 for overflow protection
static float rpm_buf[256] = {0};
static uint8_t rpm_buf_ptr = 0;

static GPIO_TypeDef *buttonPorts[4] = { BTN1_GPIO_Port, BTN2_GPIO_Port,
		BTN3_GPIO_Port, BTN4_GPIO_Port };
static uint16_t buttonPins[4] = { BTN1_Pin, BTN2_Pin, BTN3_Pin, BTN4_Pin };
static uint8_t pressed[4] = { 0 };
static uint8_t lastButtons[4] = { 0 };

static uint8_t spokes = SPOKES_DEFAULT;
//number of frames
static uint16_t timeout = 60*UPDATE_FREQ/SPOKES_DEFAULT;
static uint16_t timeout_count = 0;

void TAC_CounterHandler(void) {
	counter++;
}

void TAC_IrqHandler(void) {
	//store counter in variable because volatile
	const uint32_t count_temp = counter;

	//checks if counter has wrapped around
	if (count_temp < last_counter) {
		last_counter = count_temp;
		return;
	}

	//cycles passed
	const uint32_t passed = count_temp - last_counter;
	if (passed > TAC_MIN_TIM) {
		float rpm_calc = 12000000;
		rpm_calc /= spokes;
		rpm_calc /= passed;
		rpm_buf[rpm_buf_ptr++] = rpm_calc;
		last_counter = count_temp;
	}
}

void TAC_UpdateHandler(void) {
	if (!rpm_buf_ptr) {
		if (++timeout_count >= timeout) {
			rpm = 0;
		}
		return;
	}
	timeout_count = 0;

	float total = 0;
	for (uint8_t i = 0; i < rpm_buf_ptr; i++) {
		total += rpm_buf[i];
	}
	rpm = total/rpm_buf_ptr;
	rpm_buf_ptr = 0;
}

void Control_Init(void) {
	ssd1306_Init();
	ssd1306_SetCursor(20, 10);
	ssd1306_WriteString("Spokes:", Font_7x10, 1);
	ssd1306_SetCursor(20, 30);
	ssd1306_WriteString("RPM:", Font_7x10, 1);
	eeprom_random_read(0, &spokes, 1);
	HAL_Delay(2);

}
void Control_Loop(void) {
	uint8_t buttons[4] = { 0 };
	for (uint8_t i = 0; i < 4; i++) {
		buttons[i] = !HAL_GPIO_ReadPin(buttonPorts[i], buttonPins[i]);
		if (buttons[i] && !lastButtons[i]) {
			pressed[i] = 1;
		} else {
			pressed[i] = 0;
		}
		lastButtons[i] = buttons[i];
	}

	if (pressed[1]) {
		spokes++;
		eeprom_write(0, &spokes, 1);
		HAL_Delay(2);
		timeout = 60*UPDATE_FREQ/spokes;
	} else if (pressed[0]) {
		spokes--;
		eeprom_write(0, &spokes, 1);
		HAL_Delay(2);
		timeout = 60*UPDATE_FREQ/spokes;
	}
	ssd1306_SetCursor(70, 10);
	char buf[16];
	sprintf(buf, "%3d", spokes);
	ssd1306_WriteString(buf, Font_7x10, 1);

	ssd1306_SetCursor(50, 30);
	sprintf(buf, "%5d", rpm);
	ssd1306_WriteString(buf, Font_7x10, 1);

	ssd1306_UpdateScreen();
	HAL_Delay(10);
}
