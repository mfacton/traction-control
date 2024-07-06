#include "display.h"
#include "control.h"
#include "ssd1306.h"
#include "bitmaps.h"
#include "stdio.h"
#include "input.h"
#include "gps.h"
#include "tach.h"

extern int32_t adc_count;

static void trigger_ratio_update() {
	ssd1306_FillRectangle(44, 34, 85, 43, 0);
	if (Memory_ReadByte(MemGPSMode) && !GPS_Fix()) {
		ssd1306_SetCursor(44, 34);
		ssd1306_WriteString("No Fix", Font_7x10, 1);
	}else{
		ssd1306_SetCursor(51, 34);
		char buf[MAX_CHARACTERS];
		sprintf(buf, "%04.0f", Control_Ratio()*1000.0);
		ssd1306_WriteString(buf, Font_7x10, 1);
		ssd1306_FillCircle(57, 43, 1, 1);
	}
}

static void trigger_live_update() {
	ssd1306_FillRectangle(44, 34, 85, 43, 0);
	if (Memory_ReadByte(MemGPSMode) && !GPS_Fix()) {
		ssd1306_SetCursor(44, 34);
		ssd1306_WriteString("No Fix", Font_7x10, 1);
	}else{
		ssd1306_SetCursor(51, 34);
		char buf[MAX_CHARACTERS];
		sprintf(buf, "%04.0f", Control_Slip()*10.0);
		ssd1306_WriteString(buf, Font_7x10, 1);
		ssd1306_FillCircle(71, 43, 1, 1);
	}
}

static void rpm_live_update(uint8_t chan) {
	ssd1306_FillRectangle(48, 34, 75, 43, 0);
	ssd1306_SetCursor(48, 34);
	char buf[MAX_CHARACTERS];
	sprintf(buf, "%4d", Tach_RPM(chan));
	ssd1306_WriteString(buf, Font_7x10, 1);
}

static void tach_live_update(void) {
	rpm_live_update(TachEngine);
}

static void sensor1_live_update(void) {
	rpm_live_update(TachSensor1);
}

static void sensor2_live_update(void) {
	rpm_live_update(TachSensor2);
}

static void gps_live_update(void) {
	ssd1306_FillRectangle(44, 34, 85, 43, 0);
	if (GPS_Fix()) {
		ssd1306_SetCursor(51, 34);
		char buf[MAX_CHARACTERS];
		sprintf(buf, "%4.0f", GPS_Speed()*10.0);
		ssd1306_WriteString(buf, Font_7x10, 1);
		ssd1306_FillCircle(71, 43, 1, 1);
	}else{
		ssd1306_SetCursor(43, 34);
		ssd1306_WriteString("No Fix", Font_7x10, 1);
	}
}

static void adc_live_update(void) {
	ssd1306_FillRectangle(0, 34, 127, 43, 0);
	ssd1306_SetCursor(24, 34);
	char buf[MAX_CHARACTERS];
	sprintf(buf, "%lu", adc_count);
	adc_count = 0;
	ssd1306_WriteString(buf, Font_7x10, 1);
	HAL_Delay(980);
}

static struct Screen trigger_thresh = {.type = ScreenEdit, .memType = MemFloat, .loc = MemSlipThresh};
static struct Screen trigger_aux = {.type = ScreenEdit, .memType = MemShort, .loc = MemRPMThresh};
static struct Screen trigger_min_rpm = {.type = ScreenEdit, .memType = MemShort, .loc = MemMinRPM};
static struct Screen trigger_ratio = {.type = ScreenLive, .update = &trigger_ratio_update};
static struct Screen trigger_live = {.type = ScreenLive, .update = &trigger_live_update};
static struct Option trigger_options[5] = {
		{.type = OptionRedirect, .text = "Slip Thresh", .redirect = &trigger_thresh},
		{.type = OptionRedirect, .text = "Aux Thresh", .redirect = &trigger_aux},
		{.type = OptionRedirect, .text = "Min RPM", .redirect = &trigger_min_rpm},
		{.type = OptionRedirect, .text = "Ratio", .redirect = &trigger_ratio},
		{.type = OptionRedirect, .text = "Live", .redirect = &trigger_live}};
static struct Screen trigger = {.type = ScreenScroll, .optionCount = 5, .options = trigger_options};

static struct Screen gps_tire_circ = {.type = ScreenEdit, .memType = MemFloat, .loc = MemTireCirc};
static struct Screen gps_min_speed = {.type = ScreenEdit, .memType = MemFloat, .loc = MemMinSpeed};
static struct Screen gps_live = {.type = ScreenLive, .update = &gps_live_update};
static struct Option gps_options[3] = {
		{.type = OptionRedirect, .text = "Tire Circ", .redirect = &gps_tire_circ},
		{.type = OptionRedirect, .text = "Min Speed", .redirect = &gps_min_speed},
		{.type = OptionRedirect, .text = "Live", .redirect = &gps_live}};
static struct Screen gps = {.type = ScreenScroll, .optionCount = 3, .options = gps_options};

static struct Screen tach_ppr = {.type = ScreenEdit, .memType = MemByte, .loc = MemPPR1};
static struct Screen tach_max_rpm = {.type = ScreenEdit, .memType = MemShort, .loc = MemMaxRPM1};
static struct Screen tach_live = {.type = ScreenLive, .update = &tach_live_update};
static struct Screen tach_threshold = {.type = ScreenEdit, .memType = MemShort, .loc = MemTachThresh};
static struct Option tach_options[4] = {
		{.type = OptionRedirect, .text = "PPR", .redirect = &tach_ppr},
		{.type = OptionRedirect, .text = "Max RPM", .redirect = &tach_max_rpm},
		{.type = OptionRedirect, .text = "Live", .redirect = &tach_live},
		{.type = OptionRedirect, .text = "Threshold", .redirect = &tach_threshold}};
static struct Screen tach = {.type = ScreenScroll, .optionCount = 4, .options = tach_options};

static struct Screen sensor1_ppr = {.type = ScreenEdit, .memType = MemByte, .loc = MemPPR2};
static struct Screen sensor1_max_rpm = {.type = ScreenEdit, .memType = MemShort, .loc = MemMaxRPM2};
static struct Screen sensor1_live = {.type = ScreenLive, .update = &sensor1_live_update};
static struct Option sensor1_options[3] = {
		{.type = OptionRedirect, .text = "PPR", .redirect = &sensor1_ppr},
		{.type = OptionRedirect, .text = "Max RPM", .redirect = &sensor1_max_rpm},
		{.type = OptionRedirect, .text = "Live", .redirect = &sensor1_live}};
static struct Screen sensor1 = {.type = ScreenScroll, .optionCount = 3, .options = sensor1_options};

static struct Screen sensor2_ppr = {.type = ScreenEdit, .memType = MemByte, .loc = MemPPR3};
static struct Screen sensor2_max_rpm = {.type = ScreenEdit, .memType = MemShort, .loc = MemMaxRPM3};
static struct Screen sensor2_live = {.type = ScreenLive, .update = &sensor2_live_update};
static struct Option sensor2_options[3] = {
		{.type = OptionRedirect, .text = "PPR", .redirect = &sensor2_ppr},
		{.type = OptionRedirect, .text = "Max RPM", .redirect = &sensor2_max_rpm},
		{.type = OptionRedirect, .text = "Live", .redirect = &sensor2_live}};
static struct Screen sensor2 = {.type = ScreenScroll, .optionCount = 3, .options = sensor2_options};

static struct Screen adc_live = {.type = ScreenLive, .update = &adc_live_update};

static struct Option menu_options[8] = {
		{.type = OptionToggle, .text = "Mode", .toggleLoc = MemGPSMode, .on = "[GPS]", .off = "[2SR]"},
		{.type = OptionRedirect, .text = "Trigger", .redirect = &trigger},
		{.type = OptionRedirect, .text = "GPS", .redirect = &gps},
		{.type = OptionRedirect, .text = "Tach", .redirect = &tach},
		{.type = OptionRedirect, .text = "Sensor1", .redirect = &sensor1},
		{.type = OptionRedirect, .text = "Sensor2", .redirect = &sensor2},
		{.type = OptionAction, .text = "Reset", .action = &Memory_Reset},
		{.type = OptionRedirect, .text = "ADC Hz", .redirect = &adc_live}};
static struct Screen menu = {.type = ScreenScroll, .optionCount = 8, .options =
		menu_options, };

static struct Screen *stack[MAX_STACK] = {&menu};
static uint8_t stackIndex = 0;

//edit variables
static int8_t incrementIndex;
static float increment;
static float valBuf;

static uint8_t int_max(uint8_t a, uint8_t b) {
	if (a > b) {
		return a;
	}
	return b;
}

static void display_update_cursor(void) {
	ssd1306_FillRectangle(10, 16, 17, 63, 0);
	ssd1306_SetCursor(10, 16 * (stack[stackIndex]->select - stack[stackIndex]->scroll + 1) + 6);
	ssd1306_WriteString(">", Font_7x10, 1);
}

static void display_update_options(void) {
	for (uint8_t i = 0; i < 3; i++) {
		ssd1306_FillRectangle(20, 16 * (i + 1), 121, 16 * (i + 1) + 15, 0);
		if (i < stack[stackIndex]->optionCount) {
			ssd1306_SetCursor(20, 16 * (i + 1) + 6);
			ssd1306_WriteString(stack[stackIndex]->options[stack[stackIndex]->scroll+i].text, Font_7x10, 1);
			if (stack[stackIndex]->options[stack[stackIndex]->scroll+i].type == OptionToggle) {
				if (Memory_ReadByte(stack[stackIndex]->options[stack[stackIndex]->scroll+i].toggleLoc)) {
					ssd1306_WriteString(stack[stackIndex]->options[stack[stackIndex]->scroll+i].on, Font_7x10, 1);
				}else{
					ssd1306_WriteString(stack[stackIndex]->options[stack[stackIndex]->scroll+i].off, Font_7x10, 1);
				}
			}
		}
	}
	if (stack[stackIndex]->optionCount > 3) {
		ssd1306_FillRectangle(124, 18, 125, 61, 0);
		ssd1306_DrawBitmap(122, 16, bm_scroll, 6, 48, 1);
		//unit = 44/option_count
		//start = 18 + unit*scroll
		//end = 18 + unit*(scroll+3) - 1
		const uint8_t start = 18 + 44*stack[stackIndex]->scroll/stack[stackIndex]->optionCount;
		const uint8_t end = 17 + 44*(stack[stackIndex]->scroll+3)/stack[stackIndex]->optionCount;
		ssd1306_FillRectangle(124, start, 125, end, 1);
	}else{
		ssd1306_FillRectangle(122, 16, 127, 63, 0);
	}
}

static void display_load_variable(void) {
	if (stack[stackIndex]->memType == MemByte) {
		valBuf = Memory_ReadByte(stack[stackIndex]->loc);
	}else if (stack[stackIndex]->memType == MemShort) {
		valBuf = Memory_ReadShort(stack[stackIndex]->loc);
	}else if (stack[stackIndex]->memType == MemFloat) {
		valBuf = Memory_ReadFloat(stack[stackIndex]->loc);
	}
}

static void display_save_variable(void) {
	if (stack[stackIndex]->memType == MemByte) {
		Memory_WriteByte(stack[stackIndex]->loc, valBuf);
	}else if (stack[stackIndex]->memType == MemShort) {
		Memory_WriteShort(stack[stackIndex]->loc, valBuf);
	}else if (stack[stackIndex]->memType == MemFloat) {
		Memory_WriteFloat(stack[stackIndex]->loc, valBuf);
	}
}

static void display_update_increment(void) {
	ssd1306_FillRectangle(44, 37, 86, 38, 0);
	ssd1306_DrawBitmap(72-7*incrementIndex, 37, bm_increment, 7, 2, 1);
}

static void display_update_variable(void) {
//	ssd1306_FillRectangle(48, 40, 82, 47, 0);
	ssd1306_SetCursor(44, 40);
	char buf[MAX_CHARACTERS];
	if (stack[stackIndex]->memType == MemFloat) {
		sprintf(buf, "  %04.0f", valBuf*10.0);
		ssd1306_WriteString(buf, Font_7x10, 1);
		ssd1306_FillCircle(79, 49, 1, 1);
	}else if (stack[stackIndex]->memType == MemShort) {
		sprintf(buf, "%05.0f", valBuf);
		ssd1306_WriteString(buf, Font_7x10, 1);
	}else{
		sprintf(buf, "  %03.0f", valBuf);
		ssd1306_WriteString(buf, Font_7x10, 1);
	}
}

static void display_init_screen(void) {
	ssd1306_FillRectangle(0, 16, 127, 63, 0);
	if (stack[stackIndex]->type == ScreenScroll) {
		display_update_options();
		display_update_cursor();
	}else if (stack[stackIndex]->type == ScreenEdit) {
		incrementIndex = 0;
		increment = 1;
		display_load_variable();
		display_update_increment();
		display_update_variable();
	}else if (stack[stackIndex]->type == ScreenLive) {
		//DO NOTHING
	}
}

void Display_Init(void) {
	ssd1306_Init();
	ssd1306_DrawBitmap(0, 0, bm_logo, 128, 16, 1);
	ssd1306_DrawBitmap(120, 0, bm_no_signal, 8, 8, 1);
	display_init_screen();
	ssd1306_UpdateScreen();
}

void Display_Update(void) {
	if (stack[stackIndex]->type == ScreenScroll) {
		if (Input_Pressed(ButtonUp)) {
			if (stack[stackIndex]->select == stack[stackIndex]->scroll && stack[stackIndex]->optionCount > 3) {
				if (!stack[stackIndex]->select) {
					stack[stackIndex]->select = stack[stackIndex]->optionCount - 1;
					stack[stackIndex]->scroll = int_max(2, stack[stackIndex]->select) - 2;
					display_update_cursor();
				} else {
					stack[stackIndex]->select--;
					stack[stackIndex]->scroll--;
				}
				display_update_options();
			} else {
				if (!stack[stackIndex]->select) {
					stack[stackIndex]->select = stack[stackIndex]->optionCount - 1;
				} else {
					stack[stackIndex]->select--;
				}
				display_update_cursor();
			}
			ssd1306_UpdateScreen();
		}else if (Input_Pressed(ButtonDown)) {
			if (stack[stackIndex]->select == stack[stackIndex]->scroll + 2 && stack[stackIndex]->optionCount > 3) {
				if (stack[stackIndex]->select == stack[stackIndex]->optionCount - 1) {
					stack[stackIndex]->select = 0;
					stack[stackIndex]->scroll = 0;
					display_update_cursor();
				} else {
					stack[stackIndex]->select++;
					stack[stackIndex]->scroll++;
				}
				display_update_options();
			} else {
				if (stack[stackIndex]->select == stack[stackIndex]->optionCount-1) {
					stack[stackIndex]->select = 0;
				}else{
					stack[stackIndex]->select++;
				}
				display_update_cursor();
			}
			ssd1306_UpdateScreen();
		}else if (Input_Pressed(ButtonEnter)) {
			if (stack[stackIndex]->options[stack[stackIndex]->select].type == OptionRedirect) {
				stack[stackIndex+1] = stack[stackIndex]->options[stack[stackIndex]->select].redirect;
				stackIndex++;
				display_init_screen();
			}else if (stack[stackIndex]->options[stack[stackIndex]->select].type == OptionToggle) {
				const uint8_t loc = stack[stackIndex]->options[stack[stackIndex]->select].toggleLoc;
				Memory_WriteByte(loc, !Memory_ReadByte(loc));
				display_update_options();
			}else if (stack[stackIndex]->options[stack[stackIndex]->select].type == OptionAction) {
				(*stack[stackIndex]->options[stack[stackIndex]->select].action)();
			}
			ssd1306_UpdateScreen();
		}else if (Input_Pressed(ButtonBack)) {
			stack[stackIndex]->scroll = 0;
			stack[stackIndex]->select = 0;
			if (stackIndex > 0) {
				stackIndex--;
			}
			display_init_screen();
			ssd1306_UpdateScreen();
		}
	}else if (stack[stackIndex]->type == ScreenEdit) {
		if (Input_Pressed(ButtonDown)) {
			valBuf += increment;
			if (stack[stackIndex]->memType == MemByte && valBuf > 255) {
				valBuf = 255;
			}
			if (stack[stackIndex]->memType == MemShort && valBuf > 65535) {
				valBuf = 65535;
			}
			display_update_variable();
			ssd1306_UpdateScreen();
		}else if (Input_Pressed(ButtonUp)) {
			valBuf -= increment;
			if (valBuf < 0) {
				valBuf = 0;
			}
			display_update_variable();
			ssd1306_UpdateScreen();
		}else if (Input_Pressed(ButtonEnter)) {
			incrementIndex++;
			if (stack[stackIndex]->memType == MemFloat) {
				if (incrementIndex > 2) {
					incrementIndex = -1;
				}
			}else if (stack[stackIndex]->memType == MemByte){
				if (incrementIndex > 2) {
					incrementIndex = 0;
				}
			}else{
				if (incrementIndex > 4) {
					incrementIndex = 0;
				}
			}
			increment = 1;
			for (uint8_t i = 0; i < incrementIndex; i++) {
				increment *= 10;
			}
			for (int8_t i = incrementIndex; i < 0; i++) {
				increment /= 10;
			}
			display_update_increment();
			ssd1306_UpdateScreen();
		}else if (Input_Pressed(ButtonBack)) {
			display_save_variable();
			stackIndex--;
			display_init_screen();
			ssd1306_UpdateScreen();
		}
	}else if (stack[stackIndex]->type == ScreenLive) {
		(*stack[stackIndex]->update)();
		if (Input_Pressed(ButtonBack)) {
			stackIndex--;
			display_init_screen();
		}
		ssd1306_UpdateScreen();
	}
}

void Display_UpdateGPS(void) {
	ssd1306_FillRectangle(120, 0, 127, 7, 0);
	if (GPS_Fix()) {
		ssd1306_DrawBitmap(120, 0, bm_signal, 8, 8, 1);
	}else{
		ssd1306_DrawBitmap(120, 0, bm_no_signal, 8, 8, 1);
	}
	ssd1306_UpdateScreen();
}
