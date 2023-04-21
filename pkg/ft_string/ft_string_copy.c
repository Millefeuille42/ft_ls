//
// Created by millefeuille on 4/21/23.
//

#include "ft_string.h"

void ft_string_copy(const char *src, char *dest) {
	for (size_t i = 0; src[i]; i++)
		dest[i] = src[i];
	dest[ft_strlen(src)] = '\0';
}
