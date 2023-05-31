//
// Created by millefeuille on 4/24/23.
//

#include "ft_string.h"

char *get_after_last_sep(char *str, char sep) {
	if (!str)
		return NULL;

	size_t i = 0;
	size_t last_sep = 0;
	for (; str[i]; i++) {
		if (str[i] == sep)
			last_sep = i;
	}

	if (last_sep == 0)
		return str;

	return str + last_sep + 1;
}

char *get_after_n_sep(char *str, char sep, int n) {
	if (!str)
		return NULL;

	size_t i = 0;
	int counter = 0;
	for (; str[i] && counter < n; i++) {
		if (str[i] == sep)
			counter++;
	}

	if (!str[i])
		return str;
	return str + i;
}
