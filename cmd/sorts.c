//
// Created by millefeuille on 4/24/23.
//

#include "ft_ls.h"

static int is_alpha_valid(char c) {
	if (ft_is_alnum(c))
		return 1;
	if (c == '$')
		return 1;
	return 0;
}

char string_alpha_sort(void *a, void *b) {
	char *a_string = a;
	char *b_string = b;

	size_t a_i = 0;
	size_t b_i = 0;

	for (; a_string[a_i] && b_string[b_i];) {
		for (; a_string[a_i] && !is_alpha_valid(a_string[a_i]); a_i++);
		for (; b_string[b_i] && !is_alpha_valid(b_string[b_i]); b_i++);
		char a_char = (char) ft_tolower(a_string[a_i]);
		char b_char = (char) ft_tolower(b_string[b_i]);
		if (a_char != b_char)
			return (char) (a_char < b_char);
		a_i++;
		b_i++;
	}
	if (!a_string[a_i])
		return 1;
	return 0;
}

char file_name_alpha_sort(void *a, void *b) {
	file_data *a_file = a;
	file_data *b_file = b;

	size_t a_i = 0;
	size_t b_i = 0;

	if (!ft_strcmp(a_file->file_info->d_name, "."))
		return 1;
	if (!ft_strcmp(b_file->file_info->d_name, "."))
		return 0;

	for (; a_file->path[a_i] && b_file->path[b_i];) {
		for (; a_file->path[a_i] && !is_alpha_valid(a_file->path[a_i]); a_i++);
		for (; b_file->path[b_i] && !is_alpha_valid(b_file->path[b_i]); b_i++);
		char a_char = (char) ft_tolower(a_file->path[a_i]);
		char b_char = (char) ft_tolower(b_file->path[b_i]);
		if (a_char != b_char)
			return (char) (a_char < b_char);
		a_i++;
		b_i++;
	}
	if (!a_file->path[a_i])
		return 1;
	return 0;
}

char file_ctime_desc_sort_path(void *a, void *b) {
	char *a_path = a;
	char *b_path = b;

	struct stat a_stat;
	struct stat b_stat;
	if (lstat(a_path, &a_stat) < 0)
		return 0;
	if (lstat(b_path, &b_stat) < 0)
		return 0;

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
