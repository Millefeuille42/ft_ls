//
// Created by millefeuille on 4/21/23.
//

#include "ft_memory.h"

void del_array(void **ptr) {
	char **array = *ptr;
	for (; *array; array++)
		safe_free((void **) array);
	safe_free(ptr);
}
