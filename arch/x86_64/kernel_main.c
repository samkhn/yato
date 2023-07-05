#include "device/video/console/console.h"

void kernel_main(void)
{
	struct VGAConsole console;
	VGAConsole_Initialize(&console, kVgaConsoleDefaultWidth,
			      kVgaConsoleDefaultHeight,
			      (VGAConsole_Pixel *)kVgaConsoleDefaultFBLocation,
			      VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	VGAConsole_Write(&console, "Hello yato");
}
