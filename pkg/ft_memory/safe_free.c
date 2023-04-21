//
// Created by millefeuille on 4/21/23.
//

#include "ft_memory.h"

void safe_free(void **ptr) {
    if (!ptr || !*ptr)
        return;
    free(*ptr);
    *ptr = NULL;
}
