//
// Created by millefeuille on 4/21/23.
//

#ifndef FT_LS_FT_PRINT_H
# define FT_LS_FT_PRINT_H

# include <unistd.h>

void ft_putstr(const char *str);
void ft_fputstr(const char *str, int fd);

void ft_putchar(char c);
void ft_fputchar(char c, int fd);

void ft_putnbr(int n);
void ft_fputnbr(int n, int fd);

#endif //FT_LS_FT_PRINT_H
