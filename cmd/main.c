//
// Created by mlabouri on 4/18/23.
//

#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "../pkg/ft_error/ft_error.h"
#include "../pkg/ft_list/ft_list.h"
#include "../pkg/ft_string/ft_string.h"

#define LS_FLAG_l 1  	// 000001
#define LS_FLAG_a 2  	// 000010
#define LS_FLAG_r 4  	// 000100
#define LS_FLAG_R 8  	// 001000
#define LS_FLAG_t 16 	// 010000
#define LS_FLAG_dirs 32 // 100000 Should have multi directory display ?

#define LS_SET_FLAG_l(flags) (flags |= LS_FLAG_l)
#define LS_SET_FLAG_a(flags) (flags |= LS_FLAG_a)
#define LS_SET_FLAG_r(flags) (flags |= LS_FLAG_r)
#define LS_SET_FLAG_R(flags) (flags |= LS_FLAG_R)
#define LS_SET_FLAG_t(flags) (flags |= LS_FLAG_t)
#define LS_SET_FLAG_dirs(flags) (flags |= LS_FLAG_dirs)

#define LS_HAS_FLAG_l(flags) (flags & LS_FLAG_l)
#define LS_HAS_FLAG_a(flags) (flags & LS_FLAG_a)
#define LS_HAS_FLAG_r(flags) (flags & LS_FLAG_r)
#define LS_HAS_FLAG_R(flags) (flags & LS_FLAG_R)
#define LS_HAS_FLAG_t(flags) (flags & LS_FLAG_t)
#define LS_HAS_FLAG_dirs(flags) (flags & LS_FLAG_dirs)

typedef struct s_ls_args {
	char flags;
	ft_list *directories;
	char err;
} ls_args;

typedef struct s_file_data {
	struct dirent *file_info;
	struct stat file_stat;
	char *path;
} file_data;

enum e_ls_print_func {
	LS_PRINT_TYPE,
	LS_PRINT_PERMISSIONS,
	LS_PRINT_HARD_LINKS,
	LS_PRINT_OWNER_USER,
	LS_PRINT_OWNER_GROUP,
	LS_PRINT_SIZE,
	LS_PRINT_CTIME,
	LS_PRINT_NAME,
	LS_PRINT_LINK
};

typedef char (*dir_func)(char flags, ft_list *file_ptr);
typedef char (*file_print_func)(file_data *file);

void del_file_data(void **ptr) {
	file_data *file = (file_data *) *ptr;
	safe_free((void **) &file->path);
	safe_free(ptr);
}

char *get_path(char *s) {
	if (s)
		return ft_string(s);
	return ft_string(".");
}

char file_name_alpha_asc_sort(void *a, void *b) {
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

file_data *get_data_from_file(struct dirent *file_info, char *path) {
	char *filepath = ft_string_concat((char *[3]){path, "/", file_info->d_name}, 3);
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
	*file = (file_data){.file_info = file_info, .file_stat = file_stat, .path = filepath};
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

char iter_dir(char flags, char *path, DIR *dir, dir_func f) {
	if (!path || !dir)
		return -1;

	struct dirent *file_info = readdir(dir);
	ft_list *file_list = NULL;
	for (; file_info; file_info = readdir(dir)) {
		file_data *file = get_data_from_file(file_info, path);
		if (!file)
			return -1;
		if (file->file_info->d_name[0] == '.' && !LS_HAS_FLAG_a(flags)) {
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

	if (LS_HAS_FLAG_dirs(flags)) {
		ft_putstr(path);
		ft_putstr(":\n");
	}

	if (LS_HAS_FLAG_l(flags)) {
		size_t blocks = get_total_blocks(file_list);
		ft_putstr("total ");
		ft_putnbr((int)blocks);
		ft_putchar('\n');
	}

	file_list = list_sort(file_list, file_name_alpha_asc_sort, LS_HAS_FLAG_r(flags));

	char err = 0;
	ft_list *current = file_list;
	for (; current && !err; current = current->next)
		err = f(flags, current);

	delete_list_forward(&file_list, del_file_data);
	return err;
}

void walk_dir(DIR *dir, dir_func f) {
	(void) dir;
	(void) f;
}

char print_file_permissions(file_data *file) {
	ft_putchar((file->file_stat.st_mode & S_IRUSR) ? 'r' : '-'); /* User Read     */
	ft_putchar((file->file_stat.st_mode & S_IWUSR) ? 'w' : '-'); /* User Write    */
	ft_putchar((file->file_stat.st_mode & S_IXUSR) ? 'x' : '-'); /* User eXecute  */

	ft_putchar((file->file_stat.st_mode & S_IRGRP) ? 'r' : '-'); /* Group Read    */
	ft_putchar((file->file_stat.st_mode & S_IWGRP) ? 'w' : '-'); /* Group Write   */
	ft_putchar((file->file_stat.st_mode & S_IXGRP) ? 'x' : '-'); /* Group eXecute */

	ft_putchar((file->file_stat.st_mode & S_IROTH) ? 'r' : '-'); /* Other Read    */
	ft_putchar((file->file_stat.st_mode & S_IWOTH) ? 'w' : '-'); /* Other Write   */
	ft_putchar((file->file_stat.st_mode & S_IXOTH) ? 'x' : '-'); /* Other eXecute */

	ft_putchar('\t');
	return 0;
}

char print_file_type(file_data *file) {
	static char mode_char[7] = {"-dbcpls"};
	char mode_bool[7] = {
			S_ISREG(file->file_stat.st_mode),
			S_ISDIR(file->file_stat.st_mode),
			S_ISBLK(file->file_stat.st_mode),
			S_ISCHR(file->file_stat.st_mode),
			S_ISFIFO(file->file_stat.st_mode),
			S_ISLNK(file->file_stat.st_mode),
			S_ISSOCK(file->file_stat.st_mode),
	};

	char type = '?';
	for (int i = 0; i < 7; i++)
		if (mode_bool[i]) {
			type = mode_char[i];
			break;
		}
	ft_putchar(type);
	return 0;
}

char print_file_owner_user(file_data *file) {
	struct passwd *p_uid = getpwuid(file->file_stat.st_uid);
	if (!p_uid)
		return -1;
	ft_putstr(p_uid->pw_name);
	ft_putchar('\t');
	return 0;
}

char print_file_owner_group(file_data *file) {
	struct group *p_gid = getgrgid(file->file_stat.st_gid);
	if (!p_gid)
		return -1;
	ft_putstr(p_gid->gr_name);
	ft_putchar('\t');
	return 0;
}

char print_file_last_edit(file_data *file) {
	char *t_str = ctime(&file->file_stat.st_ctim.tv_sec);
	if (!t_str)
		return -1;
	char **t_arr = ft_split(t_str, ' ');
	if (!t_arr)
		return -1;

	for (size_t i = 0; t_arr[i]; i++) {
		if (!i || i == 4)
			continue;
		if (i == 3)
			t_arr[i][5] = '\0';
		ft_putstr(t_arr[i]);
		ft_putchar(' ');
	}

	del_array((void **) &t_arr);
	ft_putchar('\t');
	return 0;
}

char print_file_link(file_data *file) {
	if (S_ISLNK(file->file_stat.st_mode)) {
		char *buf = zeroed_malloc(1);
		if (!buf)
			return -1;

		size_t buf_size = 256;
		for (size_t read_size = buf_size + 1; read_size >= buf_size; buf_size += 256) {
			safe_free((void **) &buf);
			buf = zeroed_malloc(sizeof(char) * buf_size);
			if (!buf)
				return -1;
			read_size = readlink(file->path, buf, buf_size - 1);
		}
		ft_putstr(" -> ");
		ft_putstr(buf);
	}
	return 0;
}

char print_file_hardlink(file_data *file) {
	ft_putnbr((int) file->file_stat.st_nlink);
	ft_putchar('\t');
	return 0;
}

char print_file_size(file_data *file) {
	ft_putnbr((int) file->file_stat.st_size);
	ft_putchar('\t');
	return 0;
}

char print_file_name(file_data *file) {
	ft_putstr(get_after_last_sep(file->path, '/'));
	return 0;
}

char print_file_details(file_data *file) {
	static file_print_func print[] = {
			print_file_type,
			print_file_permissions,
			print_file_hardlink,
			print_file_owner_user,
			print_file_owner_group,
			print_file_size,
			print_file_last_edit,
			print_file_name,
			print_file_link
	};

	for (int i = LS_PRINT_TYPE; i < LS_PRINT_LINK; i++)
		print[i](file);

	ft_putchar('\n');
	return 0;
}

char print_dir_props_no_xattr(char flags, ft_list *file_ptr) {
	if (!file_ptr)
		return -1;
	
	file_data *file = file_ptr->data;

	if (LS_HAS_FLAG_l(flags))
		return print_file_details(file);

	print_file_name(file);
	if (file_ptr->next)
		ft_putchar(' ');
	return 0;
}

int start_dir_iter(char *path, char flags) {
	DIR *dir = opendir(path);
	if (!dir)
		return -1;
	if (iter_dir(flags, path, dir, print_dir_props_no_xattr)) {
		closedir(dir);
		return -1;
	}
	closedir(dir);
	return 0;
}

char parse_flags(char flags, char *str) {
	for (int i2 = 1; str[i2]; i2++) {
		switch (str[i2]) {
			case 'l':
				LS_SET_FLAG_l(flags);
				break;
			case 'a':
				LS_SET_FLAG_a(flags);
				break;
			case 'r':
				LS_SET_FLAG_r(flags);
				break;
			case 'R':
				LS_SET_FLAG_R(flags);
				break;
			case 't':
				LS_SET_FLAG_t(flags);
				break;
			default:
				if (str[i2]) {
					ft_putstr("Invalid flag: ");
					ft_putchar(str[i2]);
				}
				break;
		}
	}
	return flags;
}

ls_args parse_args(int argc, char **argv) {
	ft_list *directories = NULL;
	ls_args ret = (ls_args){.directories = NULL, .flags = '\0', .err = -1};

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			ret.flags = parse_flags(ret.flags, argv[i]);
			continue;
		}
		char *arg = ft_string(argv[i]);
		if (!arg) {
			delete_list_forward(&directories, safe_free);
			return ret;
		}
		ft_list *new_element = new_element_to_list(directories, arg);
		if (!new_element) {
			safe_free((void **) &arg);
			delete_list_forward(&directories, safe_free);
			return ret;
		}
		if (!directories)
			directories = new_element;
	}

	ret.directories = directories;
	ret.err = 0;
	if (ret.directories && ret.directories->next)
		LS_SET_FLAG_dirs(ret.flags);
	return ret;
}

int main(int argc, char *argv[]) {
	ls_args args = parse_args(argc, argv);
	if (args.err) {
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

	ft_list *current = args.directories;
	for (; current; current = current->next) {
		if (start_dir_iter(current->data, args.flags)) {
			delete_list_forward(&args.directories, safe_free);
			panic();
		}
		if (current->next)
			ft_putchar('\n');
		ft_putchar('\n');
	}
	delete_list_forward(&args.directories, safe_free);
	return 0;
}

// TODO Cleanup
// TODO Rt flags
// TODO Unknown dir error management
