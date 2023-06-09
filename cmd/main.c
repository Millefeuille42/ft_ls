//
// Created by mlabouri on 4/18/23.
//

#include "ft_ls.h"

int should_exclude_dot(char *filename, short flags) {
	if (filename[0] != '.')
		return 0;
	if (!LS_HAS_FLAG(flags, LS_FLAG_a))
		return 1;

	int is_dot_dir = !ft_strcmp(".", filename) || !ft_strcmp("..", filename);
	if (is_dot_dir && LS_HAS_FLAG(flags, LS_FLAG_A))
		return 1;
	return 0;
}

char iter_dir(short flags, char *path, DIR *dir, dir_func f) {
	if (!path || !dir)
		return -1;

	struct dirent *file_info = readdir(dir);
	ft_list *file_list = NULL;
	for (; file_info; file_info = readdir(dir)) {
		file_data *file = get_data_from_file(file_info->d_name, path);
		if (!file) {
			if (errno == EACCES)
				continue;
			return -1;
		}
		file->file_info = file_info;
		if (should_exclude_dot(file->file_info->d_name, flags)) {
			del_file_data((void **) &file);
			continue;
		}
		ft_list *new_element = new_element_to_list(file_list, file);
		if (!new_element) {
			del_file_data((void **) &file);
			delete_list_forward(&file_list, del_file_data);
			return -1;
		}
		if (!file_list)
			file_list = new_element;
	}

	if (LS_HAS_FLAG(flags, LS_FLAG_dirs)) {
		ft_putstr(path);
		ft_putstr(":\n");
	}

	if (LS_HAS_FLAG(flags, LS_FLAG_l)) {
		size_t blocks = get_total_blocks(file_list);
		ft_putstr("total ");
		ft_putnbr((int)blocks);
		ft_putchar('\n');
	}

	if (!LS_HAS_FLAG(flags, LS_FLAG_U)) {
		if (LS_HAS_FLAG(flags, LS_FLAG_t))
			file_list = list_sort(file_list, file_ctime_desc_sort);
		else
			file_list = list_sort(file_list, file_name_alpha_sort);
		if (LS_HAS_FLAG(flags, LS_FLAG_r))
			file_list = list_revert(file_list);
	}
	char err = 0;
	ft_list *current = file_list;
	for (; current && !err; current = current->next)
		err = f(flags, current);

	delete_list_forward(&file_list, del_file_data);
	return err;
}

ft_list *get_dirs_of_dir(char *path, short flags) {
	DIR *dir = opendir(path);
	if (!dir)
		return NULL;

	struct dirent *file_info = readdir(dir);
	ft_list *dir_list = NULL;
	for (; file_info; file_info = readdir(dir)) {
		if (should_exclude_dot(file_info->d_name, flags))
			continue;
		if (is_cur_dir_or_prev_dir(file_info->d_name))
			continue;
		char *filepath = NULL;
		if (has_trailing_slash(path))
			filepath = ft_string_concat((char *[2]){path, file_info->d_name}, 2);
		else
			filepath = ft_string_concat((char *[3]){path, "/", file_info->d_name}, 3);
		if (!filepath) {
			delete_list_forward(&dir_list, safe_free);
			closedir(dir);
			return NULL;
		}

		struct stat file_stat;
		if (lstat(filepath, &file_stat) < 0) {
			closedir(dir);
			safe_free((void **) &filepath);
			delete_list_forward(&dir_list, safe_free);
			return NULL;
		}


		if (!S_ISDIR(file_stat.st_mode)) {
			safe_free((void **)&filepath);
			continue;
		}

		ft_list *cur_dir = new_element_to_list(dir_list, filepath);
		if (!cur_dir) {
			safe_free((void **) &filepath);
			delete_list_forward(&dir_list, safe_free);
			closedir(dir);
			return NULL;
		}
		if (!dir_list)
			dir_list = cur_dir;

		end_of_list(dir_list);
	}
	closedir(dir);
	return dir_list;
}

int start_dir_iter(char *path, short flags) {
	DIR *dir = opendir(path);
	if (!dir) {
		ft_fputstr("ft_ls: cannot open directory '", 2);
		ft_fputstr(path, 2);
		ft_fputstr("': ", 2);
		ft_fputstr(strerror(errno), 2);
		ft_putchar('\n');
		return -2;
	}
	if (iter_dir(flags, path, dir, print_file_props)) {
		closedir(dir);
		return -1;
	}

	if (LS_HAS_FLAG(flags, LS_FLAG_R)) {
		ft_list *dirs = get_dirs_of_dir(path, flags);
		if (dirs) {
			ft_putchar('\n');
			if (!LS_HAS_FLAG(flags, LS_FLAG_l))
				ft_putchar('\n');
		}
		if (!LS_HAS_FLAG(flags, LS_FLAG_U)) {
			if (LS_HAS_FLAG(flags, LS_FLAG_t))
				dirs = list_sort(dirs, file_ctime_desc_sort_path);
			else
				dirs = list_sort(dirs, string_alpha_sort);
			if (LS_HAS_FLAG(flags, LS_FLAG_r))
				dirs = list_revert(dirs);
		}
		ft_list *current = dirs;
		for (; current; current = current->next) {
			if (start_dir_iter(current->data, flags)) {
				delete_list_forward(&dirs, safe_free);
				closedir(dir);
				return -1;
			}
			if (current->next) {
				ft_putchar('\n');
				if (!LS_HAS_FLAG(flags, LS_FLAG_l))
					ft_putchar('\n');
			}
		}
		delete_list_forward(&dirs, safe_free);
	}
	closedir(dir);
	return 0;
}

ls_args handle_files(ls_args args) {
	ft_list *current = args.directories;
	ft_list *next = NULL;
	size_t i = 0;
	for (; current; current = next) {
		next = current->next;

		struct stat file_stat;
		if (lstat(current->data, &file_stat) < 0) {
			if (errno == 2) {
				print_no_such_file(current->data);
				if (current == args.directories)
					args.directories = args.directories->next;
				delete_element(current, safe_free);
				continue;
			} // No such file or directory
			delete_list_forward(&args.directories, safe_free);
			panic();
		}

		if (S_ISDIR(file_stat.st_mode))
			continue;

		file_data *file = get_data_from_file(current->data, "");
		if (!file) {
			delete_list_forward(&args.directories, safe_free);
			panic();
		}
		file->file_info = NULL;

		if (i > 0 && !LS_HAS_FLAG(args.flags, LS_FLAG_l))
			ft_putchar(' ');
		i++;

		print_file_props_no_list(file, args.flags);
		if (current == args.directories)
			args.directories = args.directories->next;
		delete_element(current, safe_free);
	}

	if (i > 0) {
		if (!LS_HAS_FLAG(args.flags, LS_FLAG_l))
			ft_putchar('\n');
		if (args.directories)
			ft_putchar('\n');
	}

	return args;
}

int main(int argc, char *argv[]) {
	ls_args args = parse_args(argc, argv);
	if (args.err) {
		if (args.err == -2)
			return 1;
		panic();
	}

	if (!args.directories) {
		char *dir = ft_string(".");
		if (!dir)
			panic();
		args.directories = new_list_element(dir);
		if (!args.directories) {
			safe_free((void **) &dir);
			panic();
		}
	}

	if (!LS_HAS_FLAG(args.flags, LS_FLAG_U)) {
		if (LS_HAS_FLAG(args.flags, LS_FLAG_t))
			args.directories = list_sort(args.directories, file_ctime_desc_sort_path);
		else
			args.directories = list_sort(args.directories, string_alpha_sort);
		if (LS_HAS_FLAG(args.flags, LS_FLAG_r))
			args.directories = list_revert(args.directories);
	}

	args = handle_files(args);
	ft_list *current = args.directories;
	for (; current; current = current->next) {
		if (start_dir_iter(current->data, args.flags)) {
			if (errno == EACCES)
				continue;
			delete_list_forward(&args.directories, safe_free);
			panic();
		}
		if (current->next)
			ft_putchar('\n');
		if (!LS_HAS_FLAG(args.flags, LS_FLAG_l))
			ft_putchar('\n');
	}
	delete_list_forward(&args.directories, safe_free);
	return 0;
}
