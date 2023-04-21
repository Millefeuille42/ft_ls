//
// Created by millefeuille on 4/21/23.
//

#include "ft_print.h"

void ft_fputnbr(int n, int fd) {
	long int nb = n;
	if (nb < 0) {
		nb = -nb;
		write(1, "-", 1);
	}
	if (nb > 9)
	{
		ft_fputnbr((int)(nb / 10), fd);
		nb = nb % 10;
	}
	nb = nb + 48;
	write(fd, &nb, 1);
}

void ft_putnbr(int n) {
	long int nb = n;
	if (nb < 0) {
		nb = -nb;
		write(1, "-", 1);
	}
	if (nb > 9)
	{
		ft_putnbr((int)(nb / 10));
		nb = nb % 10;
	}
	nb = nb + 48;
	write(1, &nb, 1);
}
