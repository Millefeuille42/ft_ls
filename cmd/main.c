//
// Created by mlabouri on 4/18/23.
//

#include <dirent.h>
#include <sys/stat.h>
#include <attr/xattr.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "../pkg/ft_error/ft_error.h"
#include "../pkg/ft_list/ft_list.h"
#include "../pkg/ft_string/ft_string.h"

typedef struct s_xattr_pair {
	char *name;
	char *value;
} xattr_pair;

char *get_xattr_val(char *path, char *xattr_name) {
	if (!path || !xattr_name)
		return NULL;

	ssize_t size = getxattr(path, xattr_name, NULL, 0);
	if (size < 0)
		return NULL;
	char *xattr_value = zeroed_malloc((size + 1) * sizeof(char));
	if (!xattr_value)
		return NULL;

	size = getxattr(path, xattr_name, xattr_value, size);
	if (size <= 0) {
		return NULL;
	}

	return xattr_value;
}

void delete_xattr_pair(void **ptr) {
	if (!ptr || !*ptr)
		return;
	xattr_pair *pair = *ptr;
	safe_free((void **) &pair->name);
	safe_free((void **) &pair->value);
	safe_free(ptr);
}

xattr_pair *extract_xattr(char *path, char *xattr_name) {
	if (!path || !xattr_name)
		return NULL;

	xattr_name = ft_string(xattr_name);
	if (!xattr_name) {
		return NULL;
	}

	char *xattr_value = get_xattr_val(path, xattr_name);
	if (!xattr_value) {
		safe_free((void **) &xattr_name);
	}

	xattr_pair *_xattr = zeroed_malloc(sizeof(xattr_pair));
	if (!_xattr) {
		safe_free((void **) &xattr_name);
		safe_free((void **) &xattr_value);
		return NULL;
	}

	_xattr->name = xattr_name;
	_xattr->value = xattr_value;
	return _xattr;
}

ft_list *list_dir_attributes(char *path) {
	size_t size = listxattr(path, NULL, 0);
	if (size <= 0) {
		return NULL;
	}
	char *xattr_str = zeroed_malloc(size * sizeof(char) + 1);
	if (!xattr_str)
		return NULL;

	size = listxattr(path, xattr_str, size);
	ft_list *xattr_list = NULL;
	char *xattr_name = NULL;
	for (size_t i = 0; i < size; i += ft_strlen(xattr_name)) {
		size_t offset = i ? ++i : i;
		xattr_pair *_xattr = extract_xattr(path, xattr_str + offset);
		if (!_xattr) {
			safe_free((void **) &xattr_str);
			delete_list_forward(&xattr_list, delete_xattr_pair);
			return NULL;
		}
		xattr_name = _xattr->name;

		ft_list *new_element = new_element_to_list(xattr_list, _xattr);
		if (!new_element) {
			safe_free((void **) &xattr_str);
			delete_xattr_pair((void **) &_xattr);
			delete_list_forward(&xattr_list, delete_xattr_pair);
			return NULL;
		}
		if (!xattr_list)
			xattr_list = new_element;
	}

	safe_free((void **) &xattr_str);
	return xattr_list;
}

void print_xattr(ft_list *xattr_element) {
	xattr_pair * _xattr = xattr_element->data;
	ft_putstr(_xattr->name);
	ft_putchar('=');
	ft_putstr(_xattr->value);
}

int print_dir_props(char *path, struct dirent *dir) {
	if (!path || !dir)
		return -1;

	struct stat dir_info;
	if (lstat(path, &dir_info) < 0) {
		return -1;
	}

	ft_list *xattr_list = list_dir_attributes(path);
	if (!xattr_list)
		return -1;
	iter_list_forward(xattr_list, print_xattr);
	delete_list_forward(&xattr_list, delete_xattr_pair);
	ft_putchar('\t');

	ft_putstr(dir->d_name);
	ft_putchar('\n');
	return 0;
}

/* ************************************************* */

#define LS_FLAG_l 1  // 000001
#define LS_FLAG_a 2  // 000010
#define LS_FLAG_r 4  // 000100
#define LS_FLAG_R 8  // 001000
#define LS_FLAG_t 16 // 010000
#define LS_FLAG_dirs // 100000 Should have multi directory display ?

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

	if (LS_HAS_FLAG_l(flags)) {
		size_t blocks = get_total_blocks(file_list);
		ft_putstr("total ");
		ft_putnbr((int)blocks);
		ft_putchar('\n');
	}

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
				flags |= LS_FLAG_l;
				break;
			case 'a':
				flags |= LS_FLAG_a;
				break;
			case 'r':
				flags |= LS_FLAG_r;
				break;
			case 'R':
				flags |= LS_FLAG_R;
				break;
			case 't':
				flags |= LS_FLAG_t;
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
		ft_putchar('\n');
	}
	delete_list_forward(&args.directories, safe_free);
	return 0;
}

// TODO Cleanup
// TODO Sort output
// TODO multi directory display
// TODO Rrt flags
