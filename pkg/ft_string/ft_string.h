//
// Created by millefeuille on 4/21/23.
//

#ifndef FT_LS_FT_STRING_H
# define FT_LS_FT_STRING_H

#include <unistd.h>
#include "../ft_memory/ft_memory.h"

size_t ft_strlen(const char *str);
char *ft_string(const char *str);
void ft_string_copy(const char *src, char *dest, size_t len);
char *ft_string_concat(char **str_list, size_t n);
char **ft_split(char *str, char sep);
char *get_after_n_sep(char *str, char sep, int n);
char *get_after_last_sep(char *str, char sep);

int	ft_tolower(int c);
int	ft_strcmp(const char *s1, const char *s2);
int ft_is_alpha(char c);
int ft_is_alnum(char c);

#endif //FT_LS_FT_STRING_H
