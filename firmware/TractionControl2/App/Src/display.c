#include "display.h"
#include "oled.h"
#include "bitmaps.h"
#include "stdio.h"
#include "input.h"
#include "gps.h"
#include "tach.h"
#include "control.h"

static void trigger_ratio_update() {
	Oled_ClearRectangle(44, 34, 86, 45);
	if (Memory_ReadByte(MemGPSMode) && !GPS_Fix()) {
		Oled_SetCursor(44, 34);
		Oled_WriteString("No Fix", &font_7x10);
	}else{
		Oled_SetCursor(51, 34);
		char buf[DISPLAY_MAX_CHARACTERS];
		sprintf(buf, "%04.0f", Control_Ratio()*1000.0);
		Oled_WriteString(buf, &font_7x10);
		Oled_DrawBitmap(56, 42, bm_decimal, 3, 3);
	}
}

static void trigger_live_update() {
	Oled_ClearRectangle(44, 34, 86, 45);
	if (Memory_ReadByte(MemGPSMode) && !GPS_Fix()) {
		Oled_SetCursor(44, 34);
		Oled_WriteString("No Fix", &font_7x10);
	}else{
		Oled_SetCursor(51, 34);
		char buf[DISPLAY_MAX_CHARACTERS];
		sprintf(buf, "%04.0f", Control_Slip()*10.0);
		Oled_WriteString(buf, &font_7x10);
		Oled_DrawBitmap(70, 42, bm_decimal, 3, 3);
	}
}

static void rpm_live_update(uint8_t chan) {
	Oled_ClearRectangle(48, 34, 75, 44);
	Oled_SetCursor(48, 34);
	char buf[DISPLAY_MAX_CHARACTERS];
	sprintf(buf, "%4d", Tach_RPM(chan));
	Oled_WriteString(buf, &font_7x10);
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
	Oled_ClearRectangle(44, 34, 86, 45);
	if (GPS_Fix()) {
		Oled_SetCursor(51, 34);
		char buf[DISPLAY_MAX_CHARACTERS];
		sprintf(buf, "%4.0f", GPS_Speed()*10.0);
		Oled_WriteString(buf, &font_7x10);
		Oled_DrawBitmap(70, 42, bm_decimal, 3, 3);
	}else{
		Oled_SetCursor(43, 34);
		Oled_WriteString("No Fix", &font_7x10);
	}
}

static struct Screen factory_password = {.type = ScreenEdit, .memType = MemShort, .loc = MemFactoryPassword};

static struct Screen factory_min_rpm = {.type = ScreenEdit, .memType = MemShort, .loc = MemMinRPM};
static struct Screen gps_min_speed = {.type = ScreenEdit, .memType = MemFloat, .loc = MemMinSpeed};
static struct Screen tach_threshold = {.type = ScreenEdit, .memType = MemShort, .loc = MemTachHighThresh};
static struct Screen tach_max_rpm = {.type = ScreenEdit, .memType = MemShort, .loc = MemMaxTach};
static struct Screen sensor1_max_rpm = {.type = ScreenEdit, .memType = MemShort, .loc = MemMaxSens1};
static struct Screen sensor2_max_rpm = {.type = ScreenEdit, .memType = MemShort, .loc = MemMaxSens2};
static struct Option factory_options[7] = {
		{.type = OptionAction, .text = "Reset", .action = &Memory_Reset},
		{.type = OptionRedirect, .text = "Min RPM", .redirect = &factory_min_rpm},
		{.type = OptionRedirect, .text = "Min Speed", .redirect = &gps_min_speed},
		{.type = OptionRedirect, .text = "Tach Thresh", .redirect = &tach_threshold},
		{.type = OptionRedirect, .text = "Max RPM Tach", .redirect = &tach_max_rpm},
		{.type = OptionRedirect, .text = "Max RPM Sns1", .redirect = &sensor1_max_rpm},
		{.type = OptionRedirect, .text = "Max RPM Sns2", .redirect = &sensor2_max_rpm}};
static struct Screen factory = {.type = ScreenScroll, .optionCount = 7, .options = factory_options};

static struct Screen trigger_thresh = {.type = ScreenEdit, .memType = MemFloat, .loc = MemSlipThresh};
static struct Screen trigger_aux = {.type = ScreenEdit, .memType = MemShort, .loc = MemSecondThresh};
static struct Screen trigger_ratio = {.type = ScreenLive, .update = &trigger_ratio_update};
static struct Screen trigger_live = {.type = ScreenLive, .update = &trigger_live_update};
static struct Option trigger_options[4] = {
		{.type = OptionRedirect, .text = "Slip Thresh", .redirect = &trigger_thresh},
		{.type = OptionRedirect, .text = "Aux Thresh", .redirect = &trigger_aux},
		{.type = OptionRedirect, .text = "Ratio", .redirect = &trigger_ratio},
		{.type = OptionRedirect, .text = "Live", .redirect = &trigger_live}};
static struct Screen trigger = {.type = ScreenScroll, .optionCount = 4, .options = trigger_options};

static struct Screen gps_tire_circ = {.type = ScreenEdit, .memType = MemFloat, .loc = MemTireCirc};
static struct Screen gps_live = {.type = ScreenLive, .update = &gps_live_update};
static struct Option gps_options[2] = {
		{.type = OptionRedirect, .text = "Tire Circ", .redirect = &gps_tire_circ},
		{.type = OptionRedirect, .text = "Live", .redirect = &gps_live}};
static struct Screen gps = {.type = ScreenScroll, .optionCount = 2, .options = gps_options};

static struct Screen tach_ppr = {.type = ScreenEdit, .memType = MemByte, .loc = MemPPRTach};
static struct Screen tach_live = {.type = ScreenLive, .update = &tach_live_update};
static struct Option tach_options[2] = {
		{.type = OptionRedirect, .text = "Spokes", .redirect = &tach_ppr},
		{.type = OptionRedirect, .text = "Live", .redirect = &tach_live}};
static struct Screen tach = {.type = ScreenScroll, .optionCount = 2, .options = tach_options};

static struct Screen sensor1_ppr = {.type = ScreenEdit, .memType = MemByte, .loc = MemPPRSens1};
static struct Screen sensor1_live = {.type = ScreenLive, .update = &sensor1_live_update};
static struct Option sensor1_options[2] = {
		{.type = OptionRedirect, .text = "Spokes", .redirect = &sensor1_ppr},
		{.type = OptionRedirect, .text = "Live", .redirect = &sensor1_live}};
static struct Screen sensor1 = {.type = ScreenScroll, .optionCount = 2, .options = sensor1_options};

static struct Screen sensor2_ppr = {.type = ScreenEdit, .memType = MemByte, .loc = MemPPRSens2};
static struct Screen sensor2_live = {.type = ScreenLive, .update = &sensor2_live_update};
static struct Option sensor2_options[2] = {
		{.type = OptionRedirect, .text = "Spokes", .redirect = &sensor2_ppr},
		{.type = OptionRedirect, .text = "Live", .redirect = &sensor2_live}};
static struct Screen sensor2 = {.type = ScreenScroll, .optionCount = 2, .options = sensor2_options};


static struct Option menu_options[8] = {
		{.type = OptionToggle, .text = "Mode", .toggleLoc = MemGPSMode, .on = "[GPS]", .off = "[2SR]"},
		{.type = OptionRedirect, .text = "Trigger", .redirect = &trigger},
		{.type = OptionRedirect, .text = "GPS", .redirect = &gps},
		{.type = OptionRedirect, .text = "Tach", .redirect = &tach},
		{.type = OptionRedirect, .text = "Sensor1", .redirect = &sensor1},
		{.type = OptionRedirect, .text = "Sensor2", .redirect = &sensor2},
		{.type = OptionRedirect, .text = "Factory Pwd", .redirect = &factory_password},
		{.type = OptionRedirect, .text = "Factory Menu", .redirect = &factory}};
static struct Screen menu = {.type = ScreenScroll, .optionCount = 7, .options =
		menu_options, };

static struct Screen *stack[DISPLAY_MAX_STACK] = {&menu};
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
	Oled_ClearRectangle(10, 16, 18, 64);
	Oled_SetCursor(10, 16 * (stack[stackIndex]->select - stack[stackIndex]->scroll + 1) + 6);
	Oled_WriteString(">", &font_7x10);
}

static void display_update_options(void) {
	for (uint8_t i = 0; i < 3; i++) {
		Oled_ClearRectangle(20, 16 * (i + 1), 122, 16 * (i + 1) + 16);
		if (i < stack[stackIndex]->optionCount) {
			Oled_SetCursor(20, 16 * (i + 1) + 6);
			Oled_WriteString(stack[stackIndex]->options[stack[stackIndex]->scroll+i].text, &font_7x10);
			if (stack[stackIndex]->options[stack[stackIndex]->scroll+i].type == OptionToggle) {
				if (Memory_ReadByte(stack[stackIndex]->options[stack[stackIndex]->scroll+i].toggleLoc)) {
					Oled_WriteString(stack[stackIndex]->options[stack[stackIndex]->scroll+i].on, &font_7x10);
				}else{
					Oled_WriteString(stack[stackIndex]->options[stack[stackIndex]->scroll+i].off, &font_7x10);
				}
			}
		}
	}
	if (stack[stackIndex]->optionCount > 3) {
		Oled_ClearRectangle(124, 18, 126, 62);
		Oled_DrawBitmap(122, 16, bm_scroll, 6, 48);
		//unit = 44/option_count
		//start = 18 + unit*scroll
		//end = 18 + unit*(scroll+3) - 1
		const uint8_t start = 18 + 44*stack[stackIndex]->scroll/stack[stackIndex]->optionCount;
		const uint8_t end = 18 + 44*(stack[stackIndex]->scroll+3)/stack[stackIndex]->optionCount;
		Oled_FillRectangle(124, start, 126, end);
	}else{
		Oled_ClearRectangle(122, 16, 128, 64);
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
	Oled_ClearRectangle(44, 31, 88, 33);
	Oled_DrawBitmap(72-7*incrementIndex, 31, bm_increment, 7, 2);
}

static void display_update_variable(void) {
	Oled_ClearRectangle(44, 34, 86, 44);
	Oled_SetCursor(44, 34);
	char buf[DISPLAY_MAX_CHARACTERS];
	if (stack[stackIndex]->memType == MemFloat) {
		sprintf(buf, "  %04.0f", valBuf*10.0);
		Oled_WriteString(buf, &font_7x10);
		Oled_DrawBitmap(78, 42, bm_decimal, 3, 3);
	}else if (stack[stackIndex]->memType == MemShort) {
		sprintf(buf, "%05.0f", valBuf);
		Oled_WriteString(buf, &font_7x10);
	}else{
		sprintf(buf, "  %03.0f", valBuf);
		Oled_WriteString(buf, &font_7x10);
	}
}

static void display_init_screen(void) {
	Oled_ClearRectangle(0, 16, 128, 64);
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
	Oled_DrawBitmap(0, 0, bm_logo, 128, 16);
	Oled_DrawBitmap(120, 0, bm_no_signal, 8, 8);
	menu.optionCount = 7+(Memory_ReadShort(MemFactoryPassword)==DISPLAY_FACTORY_PASSWORD);
	display_init_screen();
	Oled_Update();
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
			Oled_Update();
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
			Oled_Update();
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
			Oled_Update();
		}else if (Input_Pressed(ButtonBack)) {
			stack[stackIndex]->scroll = 0;
			stack[stackIndex]->select = 0;
			if (stackIndex > 0) {
				stackIndex--;
			}
			display_init_screen();
			Oled_Update();
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
			Oled_Update();
		}else if (Input_Pressed(ButtonUp)) {
			valBuf -= increment;
			if (valBuf < 0) {
				valBuf = 0;
			}
			display_update_variable();
			Oled_Update();
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
			Oled_Update();
		}else if (Input_Pressed(ButtonBack)) {
			display_save_variable();
			stackIndex--;
			if (stack[stackIndex] == &menu) {
				menu.optionCount = 7+(Memory_ReadShort(MemFactoryPassword)==DISPLAY_FACTORY_PASSWORD);
				menu.scroll = 4;
				menu.select = 6;
			}
			display_init_screen();
			Oled_Update();
		}
	}else if (stack[stackIndex]->type == ScreenLive) {
		(*stack[stackIndex]->update)();
		if (Input_Pressed(ButtonBack)) {
			stackIndex--;
			display_init_screen();
		}
		Oled_Update();
	}
}

void Display_UpdateGPS(void) {
	Oled_ClearRectangle(120, 0, 128, 8);
	if (GPS_Fix()) {
		Oled_DrawBitmap(120, 0, bm_signal, 8, 8);
	}else{
		Oled_DrawBitmap(120, 0, bm_no_signal, 8, 8);
	}
	Oled_Update();
}
