//
// Created by millefeuille on 4/21/23.
//

#include "ft_memory.h"

void *zeroed_malloc(size_t n) {
    void *ret = malloc(n);
    ft_bzero(ret, n);
    return ret;
}
