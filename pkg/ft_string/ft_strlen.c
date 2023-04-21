//
// Created by millefeuille on 4/21/23.
//

#include "ft_string.h"

size_t ft_strlen(const char *str) {
	if (!str)
		return 0;
    size_t i;
    for (i = 0; str[i]; i++);
    return i;
}
