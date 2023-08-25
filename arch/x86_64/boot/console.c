#include "arch/x86_64/boot/console.h"

const uint32_t VGA_CONSOLE_DEFAULT_WIDTH = 80;
const uint32_t VGA_CONSOLE_DEFAULT_HEIGHT = 25;
// TODO: this hardcoded value only works if in protected mode once handed off
// GNU Grub 2. Maybe we make this a configurable option if we ever support
// multiple bootloaders
const vga_console_pixel_t *VGA_CONSOLE_DEFAULT_FB_ADDR =
	(vga_console_pixel_t *)0xB8000;

static void itoa(char *buf, int base, int d);

static int BOOT_CONSOLE_INIT = 0;
static vga_console_t BOOT_CONSOLE;

vga_console_color_t vga_console_encode_color(enum vga_color_code fg,
					     enum vga_color_code bg)
{
	return fg | bg << 4;
}

vga_console_pixel_t vga_console_encode_pixel(unsigned char c,
					     vga_console_color_t color)
{
	return (vga_console_pixel_t)c | (vga_console_pixel_t)color << 8;
}

uint32_t vga_console_dimension_to_index(vga_console_t *console, uint32_t x,
					uint32_t y)
{
	return y * console->screen_width + x;
}

int bprintf(const char *format, ...)
{
	if (!BOOT_CONSOLE_INIT) {
		vga_console_init(
			&BOOT_CONSOLE, VGA_CONSOLE_DEFAULT_WIDTH,
			VGA_CONSOLE_DEFAULT_HEIGHT,
			(vga_console_pixel_t *)VGA_CONSOLE_DEFAULT_FB_ADDR,
			VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
		BOOT_CONSOLE_INIT = 1;
	}
	int count = 0;
	char **arg = (char **)&format;
	arg++;
	char format_iter;
	char buffer[20];
	while ((format_iter = *format++) != 0) {
		if (format_iter != '%') {
			count += 1;
			vga_console_printchar(&BOOT_CONSOLE, format_iter);
		} else {
			char *buffer_iter;
			char *buffer_end;
			int zero_padding = 0;
			int nonzero_padding = 0;
			format_iter = *format++;
			if (format_iter == '0') {
				zero_padding = 1;
				format_iter = *format++;
			}
			if (format_iter >= '0' && format_iter <= '9') {
				nonzero_padding = format_iter - '0';
				format_iter = *format++;
			}
			switch (format_iter) {
			case 'd':
			case 'u':
			case 'x':
				itoa(buffer, format_iter, *((int *)arg++));
				buffer_iter = buffer;
				goto string;
				break;
			case 's':
				buffer_iter = *arg++;
				if (!buffer_iter)
					buffer_iter = "(null)";
string:
				for (buffer_end = buffer_iter; *buffer_end;
				     buffer_end++) {
				}
				for (;
				     buffer_end < buffer_iter + nonzero_padding;
				     buffer_end++) {
					count += 1;
					vga_console_printchar(
						&BOOT_CONSOLE,
						zero_padding ? '0' : ' ');
				}
				while (*buffer_iter)
					vga_console_printchar(&BOOT_CONSOLE,
							      *buffer_iter++);
				break;
			default:
				count += 1;
				vga_console_printchar(&BOOT_CONSOLE,
						      *((int *)arg++));
				break;
			}
		}
	}
	return count;
}

int bprint(const char *data)
{
	if (!BOOT_CONSOLE_INIT) {
		vga_console_init(
			&BOOT_CONSOLE, VGA_CONSOLE_DEFAULT_WIDTH,
			VGA_CONSOLE_DEFAULT_HEIGHT,
			(vga_console_pixel_t *)VGA_CONSOLE_DEFAULT_FB_ADDR,
			VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
		BOOT_CONSOLE_INIT = 1;
	}
	return vga_console_printn(&BOOT_CONSOLE, data, strlen(data));
}

void vga_console_printchar(vga_console_t *console, char c)
{
	const uint32_t i = vga_console_dimension_to_index(
		console, console->cursor_x, console->cursor_y);
	switch (c) {
	case '\r':
	case '\n':
		goto newline;
		break;
	default:
		console->frame_buffer[i] =
			vga_console_encode_pixel(c, console->default_color);
		if (++console->cursor_x == console->screen_width) {
newline:
			console->cursor_x = 0;
			if (++console->cursor_y == console->screen_height) {
				console->cursor_y = 0;
			}
		}
	}
}

void vga_console_clear(vga_console_t *console)
{
	for (int y = 0; y < console->screen_height; ++y) {
		for (int x = 0; x < console->screen_width; ++x) {
			const uint32_t i =
				vga_console_dimension_to_index(console, x, y);
			console->frame_buffer[i] = vga_console_encode_pixel(
				' ', console->default_color);
		}
	}
}

void vga_console_init(vga_console_t *console, int width, int height,
		      uint16_t *buffer, vga_color_code_t fg,
		      vga_color_code_t bg)
{
	console->cursor_x = 0;
	console->cursor_y = 0;
	console->screen_width = width;
	console->screen_height = height;
	console->default_color = vga_console_encode_color(fg, bg);
	console->frame_buffer = buffer;
	vga_console_clear(console);
}

static void itoa(char *buf, int base, int d)
{
	char *p = buf;
	unsigned long ud = d;
	int divisor = 10;
	if (base == 'd' && d < 0) {
		*p++ = '-';
		buf++;
		ud = -d;
	} else if (base == 'x')
		divisor = 16;

	do {
		int r = ud % divisor;
		*p++ = (r < 10) ? r + '0' : r + 'a' - 10;
	} while (ud /= divisor);
	*p = 0;
	char *p1 = buf;
	char *p2 = p - 1;
	while (p1 < p2) {
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
}

int vga_console_printn(vga_console_t *console, const char *data, uint32_t len)
{
	for (uint32_t i = 0; i < len; ++i)
		vga_console_printchar(console, data[i]);
	return len;
}
