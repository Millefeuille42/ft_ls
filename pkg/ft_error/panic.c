//
// Created by millefeuille on 4/21/23.
//

#include "ft_error.h"

void panic(void) {
    log_error();
    exit(errno);
}
