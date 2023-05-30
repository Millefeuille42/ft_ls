//
// Created by millefeuille on 4/24/23.
//

#include "ft_ls.h"

char string_alpha_sort(void *a, void *b) {
	char *a_string = a;
	char *b_string = b;
	for (size_t i = 0; a_string[i]; i++) {
		char a_char = a_string[i];
		for (size_t i2 = i; !ft_is_alnum(a_char) && a_char != 0; i2++)
			a_char = a_string[i2];
		char b_char = b_string[i];
		for (size_t i2 = i; !ft_is_alnum(b_char) && b_char != 0; i2++)
			b_char = b_string[i2];
		a_char = (char) ft_tolower(a_char);
		b_char = (char) ft_tolower(b_char);
		if (a_char != b_char)
			return (char) (a_char < b_char);
	}
	return 1;
}

char file_name_alpha_sort(void *a, void *b) {
	file_data *a_file = a;
	file_data *b_file = b;
	for (size_t i = 0; a_file->path[i]; i++) {
		char a_char = a_file->path[i];
		for (size_t i2 = i; !ft_is_alnum(a_char) && a_char != 0; i2++)
			a_char = a_file->path[i2];
		char b_char = b_file->path[i];
		for (size_t i2 = i; !ft_is_alnum(b_char) && b_char != 0; i2++)
			b_char = b_file->path[i2];
		a_char = (char) ft_tolower(a_char);
		b_char = (char) ft_tolower(b_char);
		if (a_char != b_char)
			return (char) (a_char < b_char);
	}
	return 1;
}

char file_ctime_desc_sort_path(void *a, void *b) {
	char *a_path = a;
	char *b_path = b;

	struct stat a_stat;
	struct stat b_stat;
	if (lstat(a_path, &a_stat) < 0) {
		ft_putstr("LOL CPT\n");
		return 0;
	}
	if (lstat(b_path, &b_stat) < 0) {
		ft_putstr("LOL CPT\n");
		return 0;
	}

	if (b_stat.st_mtim.tv_sec == a_stat.st_mtim.tv_sec) {
		if (b_stat.st_mtim.tv_nsec == a_stat.st_mtim.tv_nsec)
			return string_alpha_sort(a, b);
		return (char) (b_stat.st_mtim.tv_nsec < a_stat.st_mtim.tv_nsec);
	}

	return (char) (b_stat.st_mtim.tv_sec < a_stat.st_mtim.tv_sec);
}


char file_ctime_desc_sort(void *a, void *b) {
	file_data *a_file = a;
	file_data *b_file = b;

	if (b_file->file_stat.st_mtim.tv_sec == a_file->file_stat.st_mtim.tv_sec) {
		if (b_file->file_stat.st_mtim.tv_nsec == a_file->file_stat.st_mtim.tv_nsec)
			return file_name_alpha_sort(a, b);
		return (char) (b_file->file_stat.st_mtim.tv_nsec < a_file->file_stat.st_mtim.tv_nsec);
	}

	return (char) (b_file->file_stat.st_mtim.tv_sec < a_file->file_stat.st_mtim.tv_sec);
}
