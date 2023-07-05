#ifndef YATO_DEVICE_VIDEO_CONSOLE_CONSOLE_H
#define YATO_DEVICE_VIDEO_CONSOLE_CONSOLE_H

#include <stdint.h>

#include "lib/string/string.h"

typedef uint8_t VGAConsole_Color;
typedef uint16_t VGAConsole_Pixel;

// Top left corner of screen is considered (0, 0)
// Bottom right corner of screen is considered (x, y)
struct VGAConsole {
	int cursor_x;
	int cursor_y;
	int screen_width;
	int screen_height;
	int default_color;
	VGAConsole_Pixel *frame_buffer;
};

enum VGAColorCode {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline VGAConsole_Color VGAConsole_EncodeColor(enum VGAColorCode fg,
						      enum VGAColorCode bg)
{
	return fg | bg << 4;
}

static inline VGAConsole_Pixel VGAConsole_EncodePixel(unsigned char c,
						      VGAConsole_Color color)
{
	return (VGAConsole_Pixel)c | (VGAConsole_Pixel)color << 8;
}

static const uint32_t kVgaConsoleDefaultWidth = 80;
static const uint32_t kVgaConsoleDefaultHeight = 25;
static const uint16_t *kVgaConsoleDefaultFBLocation = (uint16_t *)0xB8000;

static inline uint32_t VGAConsole_DimensionToIndex(struct VGAConsole *console,
						   uint32_t x, uint32_t y)
{
	return y * console->screen_width + x;
}

static void VGAConsole_WriteChar(struct VGAConsole *console, char c)
{
	const uint32_t i = VGAConsole_DimensionToIndex(
		console, console->cursor_x, console->cursor_y);
	console->frame_buffer[i] =
		VGAConsole_EncodePixel(c, console->default_color);
	if (++console->cursor_x == console->screen_width) {
		console->cursor_x = 0;
		if (++console->cursor_y == console->screen_height) {
			console->cursor_y = 0;
		}
	}
}

static void VGAConsole_ClearScreen(struct VGAConsole *console)
{
	for (int y = 0; y < console->screen_height; ++y) {
		for (int x = 0; x < console->screen_width; ++x) {
			const uint32_t i =
				VGAConsole_DimensionToIndex(console, x, y);
			console->frame_buffer[i] = VGAConsole_EncodePixel(
				' ', console->default_color);
		}
	}
}

static void VGAConsole_Initialize(struct VGAConsole *console, int width,
				  int height, uint16_t *buffer,
				  enum VGAColorCode fg, enum VGAColorCode bg)
{
	console->cursor_x = 0;
	console->cursor_y = 0;
	console->screen_width = width;
	console->screen_height = height;
	console->default_color = VGAConsole_EncodeColor(fg, bg);
	console->frame_buffer = buffer;
	VGAConsole_ClearScreen(console);
}

static int VGAConsole_WriteN(struct VGAConsole *console, const char *data,
			     uint32_t len)
{
	for (uint32_t i = 0; i < len; ++i) {
		VGAConsole_WriteChar(console, data[i]);
	}
	return len;
}

static inline int VGAConsole_Write(struct VGAConsole *console, const char *data)
{
	return VGAConsole_WriteN(console, data, strlen(data));
}

#endif // YATO_DEVICE_VIDEO_CONSOLE_CONSOLE_H
