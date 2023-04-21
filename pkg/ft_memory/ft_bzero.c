//
// Created by millefeuille on 4/21/23.
//

#include "ft_memory.h"

void ft_bzero(void *ptr, size_t n) {
    if (!ptr)
        return;
    unsigned char *buffer = (unsigned char *) ptr;
    for (size_t i = 0; i < n; i++)
        buffer[i] = 0;
}
