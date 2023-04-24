//
// Created by millefeuille on 4/24/23.
//

#include "ft_ls.h"

file_data *get_data_from_file(char *name, char *path) {
	char *filepath = NULL;
	if (has_trailing_slash(path))
		filepath = ft_string_concat((char *[2]){path, name}, 2);
	else
		filepath = ft_string_concat((char *[3]){path, "/", name}, 3);
	if (!filepath)
		return NULL;
	struct stat file_stat;
	if (lstat(filepath, &file_stat) < 0) {
		safe_free((void **) &filepath);
		return NULL;
	}
	file_data *file = zeroed_malloc(sizeof(file_data));
	if (!file) {
		safe_free((void **) &filepath);
		return NULL;
	}
	*file = (file_data){.file_stat = file_stat, .path = filepath};
	return file;
}

size_t get_total_blocks(ft_list *file_list) {
	if (!file_list)
		return 0;

	float block_divider = -1;
	size_t block_count = 0;
	ft_list *current = file_list;
	for (; current; current = current->next) {
		file_data *file = current->data;
		if (block_divider < 0)
			block_divider = (float) file->file_stat.st_blksize / 4096;
		size_t blocks = file->file_stat.st_blocks;
		if (block_divider)
			blocks = (size_t)((float)blocks / block_divider);
		block_count += blocks;
	}

	return block_count;
}
