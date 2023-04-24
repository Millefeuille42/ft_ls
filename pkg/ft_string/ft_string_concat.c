//
// Created by millefeuille on 4/21/23.
//

#include <string.h>
#include "ft_string.h"

char *ft_string_concat(char **str_list, size_t n) {
	if (!str_list)
		return NULL;

	size_t size = 1;
	for (size_t i = 0; i < n; i++)
		size += ft_strlen(str_list[i]);

	char *ret = zeroed_malloc(sizeof(char) * size);

	for (size_t i = 0; i < n; i++) {
		size_t offset = ft_strlen(ret);
		ft_string_copy(str_list[i], ret + offset, ft_strlen(str_list[i]) + 1);
	}

	return ret;
}
