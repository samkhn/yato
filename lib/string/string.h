#ifndef YATO_LIB_STRING_STRING_H
#define YATO_LIB_STRING_STRING_H

uint32_t strlen(const char *str) {
	uint32_t len = 0;
	if (!str)
		return len;
	while (str[len])
		len++;
	return len;
}

#endif // YATO_LIB_STRING_STRING_H
