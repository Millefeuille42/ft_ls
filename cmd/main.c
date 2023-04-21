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
	char *xattr_value = zeroed_malloc(size * sizeof(char) + 1);
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

	xattr_pair *_xattr = zeroed_malloc(sizeof(xattr_pair *) + sizeof(void *));
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

		if (!xattr_list) {
			xattr_list = new_list_element(_xattr);
			if (!xattr_list) {
				safe_free((void **) &xattr_str);
				delete_xattr_pair((void **) &_xattr);
				return NULL;
			}
			continue;
		}
		ft_list *new_element = new_element_to_list(xattr_list, _xattr);
		if (!new_element) {
			safe_free((void **) &xattr_str);
			delete_xattr_pair((void **) &_xattr);
			delete_list_forward(&xattr_list, delete_xattr_pair);
			return NULL;
		}
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

typedef char (*dir_func)(char flags, char *path, struct dirent *);
typedef char (*file_print_func)(const struct stat *file_info, const char *path);

typedef struct s_ls_args {
	char flags;
	ft_list *directories;
	char err;
} ls_args;

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

char *get_path(char *s) {
	if (s)
		return ft_string(s);
	return ft_string(".");
}

char iter_dir(char flags, char *path, DIR *dir, dir_func f) {
	if (!path || !dir)
		return -1;

	char err = 0;
	struct dirent *dir_data = readdir(dir);
	for (; dir_data && !err; dir_data = readdir(dir)) {
		char *filepath = ft_string_concat((char *[3]){path, "/", dir_data->d_name}, 3);
		if (!filepath)
			return -1;
		err = f(flags, filepath, dir_data);
		safe_free((void **) &filepath);
	}
	return err;
}

void walk_dir(DIR *dir, dir_func f) {
	(void) dir;
	(void) f;
}

char print_file_permissions(const struct stat *file_info, const char *path) {
	(void) path;
	ft_putchar((file_info->st_mode & S_IRUSR) ? 'r' : '-'); /* User Read     */
	ft_putchar((file_info->st_mode & S_IWUSR) ? 'w' : '-'); /* User Write    */
	ft_putchar((file_info->st_mode & S_IXUSR) ? 'x' : '-'); /* User eXecute  */

	ft_putchar((file_info->st_mode & S_IRGRP) ? 'r' : '-'); /* Group Read    */
	ft_putchar((file_info->st_mode & S_IWGRP) ? 'w' : '-'); /* Group Write   */
	ft_putchar((file_info->st_mode & S_IXGRP) ? 'x' : '-'); /* Group eXecute */

	ft_putchar((file_info->st_mode & S_IROTH) ? 'r' : '-'); /* Other Read    */
	ft_putchar((file_info->st_mode & S_IWOTH) ? 'w' : '-'); /* Other Write   */
	ft_putchar((file_info->st_mode & S_IXOTH) ? 'x' : '-'); /* Other eXecute */

	ft_putchar('\t');
	return 0;
}

char print_file_type(const struct stat *file_info, const char *path) {
	(void) path;
	static char mode_char[7] = {"-dbcpls"};
	char mode_bool[7] = {
			S_ISREG(file_info->st_mode),
			S_ISDIR(file_info->st_mode),
			S_ISBLK(file_info->st_mode),
			S_ISCHR(file_info->st_mode),
			S_ISFIFO(file_info->st_mode),
			S_ISLNK(file_info->st_mode),
			S_ISSOCK(file_info->st_mode),
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

char print_file_owner_user(const struct stat *file_info, const char *path) {
	(void) path;
	struct passwd *p_uid = getpwuid(file_info->st_uid);
	if (!p_uid)
		return -1;
	ft_putstr(p_uid->pw_name);
	ft_putchar('\t');
	return 0;
}

char print_file_owner_group(const struct stat *file_info, const char *path) {
	(void) path;
	struct group *p_gid = getgrgid(file_info->st_gid);
	if (!p_gid)
		return -1;
	ft_putstr(p_gid->gr_name);
	ft_putchar('\t');
	return 0;
}

char print_file_last_edit(const struct stat *file_info, const char *path) {
	(void) path;
	char *t_str = ctime(&file_info->st_ctim.tv_sec);
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

char print_file_link(const struct stat *file_info, const char *path) {
	if (S_ISLNK(file_info->st_mode)) {
		char *buf = zeroed_malloc(1);
		if (!buf)
			return -1;

		size_t buf_size = 256;
		for (size_t read_size = buf_size + 1; read_size >= buf_size; buf_size += 256) {
			safe_free((void **) &buf);
			buf = zeroed_malloc(sizeof(char) * buf_size);
			if (!buf)
				return -1;
			read_size = readlink(path, buf, buf_size - 1);
		}
		ft_putstr(" -> ");
		ft_putstr(buf);
	}
	return 0;
}

char print_file_hardlink(const struct stat *file_info, const char *path) {
	(void) path;
	ft_putnbr((int) file_info->st_nlink);
	ft_putchar('\t');
	return 0;
}

char print_file_size(const struct stat *file_info, const char *path) {
	(void) path;
	ft_putnbr((int) file_info->st_size);
	ft_putchar('\t');
	return 0;
}

char print_file_name(const struct stat *file_info, const char *path) {
	(void) file_info;
	ft_putstr(path);
	return 0;
}

char print_file_details(char *path) {
	struct stat file_info;
	if (lstat(path, &file_info) < 0) {
		return -1;
	}

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
		print[i](&file_info, path);

	ft_putchar('\n');
	return 0;
}

char print_dir_props_no_xattr(char flags, char *path, struct dirent *dir) {
	if (!path || !dir)
		return -1;

	if (dir->d_name[0] == '.' && !LS_HAS_FLAG_a(flags))
		return 0;

	if (LS_HAS_FLAG_l(flags)) {
		return print_file_details(path);
	}

	print_file_name(NULL, dir->d_name);
	ft_putchar(' ');
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
		if (!directories) {
			directories = new_list_element(arg);
			if (!directories) {
				safe_free((void **) &arg);
				return ret;
			}
			continue;
		}
		ft_list *new_element = new_element_to_list(directories, arg);
		if (!new_element) {
			safe_free((void **) &arg);
			delete_list_forward(&directories, safe_free);
			return ret;
		}
	}

	ret.directories = directories;
	ret.err = 0;
	return ret;
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
// TODO Do not print fullpath when printing filename
// TODO Do not put space at the end of normal listing
// TODO multi directory display
// TODO Rrt flags
