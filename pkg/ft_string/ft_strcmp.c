//
// Created by millefeuille on 4/28/23.
//

#include "ft_string.h"

int	ft_strcmp(const char *s1, const char *s2) {
	if (!s1 || !s2)
		return 0;
	while ((*s1 || *s2) && *s1 == *s2) {
		s1++;
		s2++;
	}
	return (unsigned char)*s1 - (unsigned char)*s2;
}
