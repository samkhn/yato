#include "lib/memory.h"

void *memset(void *ptr, uint8_t value, size_t len)
{
	uint8_t *p = ptr;
	while (len--)
		*p++ = (uint8_t)value;
	return ptr;
}
