//
// Created by millefeuille on 4/21/23.
//

#include "ft_string.h"

char *ft_string(const char *str) {
	if (!str)
		return NULL;
    char *ret = zeroed_malloc(sizeof(char) * (ft_strlen(str) + 1));
	ft_string_copy(str, ret);
    return ret;
}
