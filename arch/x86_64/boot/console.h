#ifndef YATO_ARCH_X86_64_BOOT_CONSOLE_H
#define YATO_ARCH_X86_64_BOOT_CONSOLE_H

#include <stdint.h>

int bprintf(const char *format, ...);
int bprint(const char *data);

typedef uint8_t vga_console_color_t;
typedef uint16_t vga_console_pixel_t;

// Top left corner of screen is considered (0, 0)
// Bottom right corner of screen is considered (x, y)
struct vga_console {
	int cursor_x;
	int cursor_y;
	int screen_width;
	int screen_height;
	int default_color;
	vga_console_pixel_t *frame_buffer;
};
typedef struct vga_console vga_console_t;

enum vga_color_code {
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
typedef enum vga_color_code vga_color_code_t;

extern const uint32_t VGA_CONSOLE_DEFAULT_WIDTH;
extern const uint32_t VGA_CONSOLE_DEFAULT_HEIGHT;
extern const vga_console_pixel_t *VGA_CONSOLE_DEFAULT_FB_ADDR;

vga_console_color_t vga_console_encode_color(enum vga_color_code fg,
					     enum vga_color_code bg);

vga_console_pixel_t vga_console_encode_pixel(unsigned char c,
					     vga_console_color_t color);

uint32_t vga_console_dimension_to_index(vga_console_t *console, uint32_t x,
					uint32_t y);

void vga_console_init(vga_console_t *console, int width, int height,
		      uint16_t *buffer, enum vga_color_code fg,
		      enum vga_color_code bg);

void vga_console_printchar(vga_console_t *console, char c);

int vga_console_printn(vga_console_t *console, const char *data, uint32_t len);

void vga_console_clear(vga_console_t *console);

// for testing only
void test_bprintf(int halt);

#endif // YATO_ARCH_X86_64_BOOT_CONSOLE_H
