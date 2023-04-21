//
// Created by millefeuille on 4/21/23.
//

#include "ft_string.h"

char **ft_split(char *str, char sep) {
	if (!str)
		return NULL;

	size_t count = 0;

	for (size_t i = 0; str[i]; i++) {
		if (str[i] == sep) {
			count++;
			for (; str[i] && str[i] == sep; i++);
			i--;
		}
	}
	count++;

	char **ret = zeroed_malloc(sizeof(char *) * (count + 1));
	if (!ret)
		return NULL;

	size_t offset = 0;
	count = 0;
	for (size_t i = 0; str[i]; i++) {
		if (str[i] == sep) {
			str[i] = '\0';
			ret[count] = ft_string(str + offset);
			if (!ret[count])
				del_array((void **) ret);
			i++;
			for (; str[i] && str[i] == sep; i++);
			offset = i;
			i--;
			count++;
		}
	}

	ret[count] = ft_string(str + offset);
	if (!ret[count])
		del_array((void **) ret);

	return ret;
}
