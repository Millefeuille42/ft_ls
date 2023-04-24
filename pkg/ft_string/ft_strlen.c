//
// Created by millefeuille on 4/21/23.
//

#include "ft_string.h"

size_t ft_strlen(const char *str) {
	const char *s;
    for (s = str; *s; ++s);
    return s - str;
}
