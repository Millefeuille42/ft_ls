//
// Created by millefeuille on 4/21/23.
//

#include "ft_print.h"

void ft_putchar(const char c) {
    write(1, &c, 1);
}

void ft_fputchar(const char c, int fd) {
	if (fd < 0)
		return;
    write(fd, &c, 1);
}
