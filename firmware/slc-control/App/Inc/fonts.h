#ifndef INC_FONTS_H_
#define INC_FONTS_H_

#include "stdint.h"

#define FONT_CHARACTERS 95
#define FONT_START 32

struct Font {
	const uint16_t width;
	const uint16_t height;
	const uint8_t scale;//default scale of 1
	const uint8_t *data;
};

extern const struct Font font_7x10;

#endif
