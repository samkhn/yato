#include "lib/string/string.h"

uint32_t strlen(const char *str) {
	uint32_t len = 0;
	if (!str)
		return len;
	while (str[len])
		len++;
	return len;
}
