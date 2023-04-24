//
// Created by millefeuille on 4/21/23.
//

#include "ft_memory.h"

void ft_bzero(void *ptr, size_t n) {
    if (!ptr)
        return;
    unsigned char *buffer = (unsigned char *) ptr;
    while (n-- > 0)
        *buffer++ = 0;
}
