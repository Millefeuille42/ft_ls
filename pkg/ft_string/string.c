//
// Created by millefeuille on 4/21/23.
//

#include "ft_string.h"

char *ft_string(const char *str) {
	if (!str)
		return NULL;
	size_t len = ft_strlen(str) + 1;
    char *ret = zeroed_malloc(sizeof(char) * len);
	ft_string_copy(str, ret, len);
    return ret;
}
