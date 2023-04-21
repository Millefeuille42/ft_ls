//
// Created by millefeuille on 4/21/23.
//

#ifndef FT_LS_FT_MEMORY_H
# define FT_LS_FT_MEMORY_H

# include <unistd.h>
# include <stdlib.h>

void ft_bzero(void *ptr, size_t n);
void safe_free(void **ptr);
void *zeroed_malloc(size_t n);
void del_array(void **ptr);

#endif //FT_LS_FT_MEMORY_H
