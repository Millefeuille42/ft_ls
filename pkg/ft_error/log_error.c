//
// Created by millefeuille on 4/21/23.
//

#include "ft_error.h"

void log_error(void) {
    ft_fputstr("Error: ", 1);
    ft_fputstr(strerror(errno), 1);
	ft_putchar('\n');
}
