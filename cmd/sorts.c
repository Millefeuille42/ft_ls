//
// Created by millefeuille on 4/24/23.
//

#include "ft_ls.h"

char file_name_alpha_sort(void *a, void *b) {
	file_data *a_file = a;
	file_data *b_file = b;
	for (size_t i = 0; a_file->path[i]; i++) {
		char a_char = (char) ft_tolower(a_file->path[i]);
		for (size_t i2 = i; a_char == '.'; i2++)
			a_char = a_file->path[i2];
		char b_char = (char) ft_tolower(b_file->path[i]);
		for (size_t i2 = i; b_char == '.'; i2++)
			b_char = b_file->path[i2];
		if (a_char != b_char)
			return (char) (a_char < b_char);
	}
	return 1;
}

char file_ctime_desc_sort(void *a, void *b) {
	file_data *a_file = a;
	file_data *b_file = b;

	return (char) (b_file->file_stat.st_ctim.tv_sec < a_file->file_stat.st_ctim.tv_sec);
}
