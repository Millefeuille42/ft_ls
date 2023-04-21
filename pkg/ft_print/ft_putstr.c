//
// Created by millefeuille on 4/18/23.
//

#include "ft_print.h"

void ft_putstr(const char *str) {
	if (!str)
		return;
    for (; *str; str++)
        write(1, str, 1);
}

void ft_fputstr(const char *str, int fd) {
	if (!str || fd < 0)
		return;
    for (; *str; str++)
        write(fd, str, 1);
}
