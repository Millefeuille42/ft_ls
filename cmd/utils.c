//
// Created by millefeuille on 4/24/23.
//

#include "ft_ls.h"

void del_file_data(void **ptr) {
	file_data *file = (file_data *) *ptr;
	safe_free((void **) &file->path);
	safe_free(ptr);
}

int has_trailing_slash(char *path) {
	size_t len = ft_strlen(path);
	return len && path[len - 1] == '/';
}

int is_cur_dir_or_prev_dir(char *dir_name) {
	size_t len = ft_strlen(dir_name);
	return dir_name && dir_name[0] == '.' && (len == 1 || (len == 2 && dir_name[1] == '.'));
}
