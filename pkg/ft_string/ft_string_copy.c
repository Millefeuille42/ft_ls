//
// Created by millefeuille on 4/21/23.
//

#include "ft_string.h"

void ft_string_copy(const char *src, char *dest, size_t len) {
	while (len--)
		*dest++ = *src++;
}
