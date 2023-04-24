//
// Created by millefeuille on 4/24/23.
//

#include "ft_ls.h"

static char print_file_permissions(file_data *file) {
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

static char print_file_type(file_data *file) {
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

static char print_file_owner_user(file_data *file) {
	struct passwd *p_uid = getpwuid(file->file_stat.st_uid);
	if (!p_uid)
		return -1;
	ft_putstr(p_uid->pw_name);
	ft_putchar('\t');
	return 0;
}

static char print_file_owner_group(file_data *file) {
	struct group *p_gid = getgrgid(file->file_stat.st_gid);
	if (!p_gid)
		return -1;
	ft_putstr(p_gid->gr_name);
	ft_putchar('\t');
	return 0;
}

static char print_file_last_edit(file_data *file) {
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

static char print_file_link(file_data *file) {
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

static char print_file_hardlink(file_data *file) {
	ft_putnbr((int) file->file_stat.st_nlink);
	ft_putchar('\t');
	return 0;
}

static char print_file_size(file_data *file) {
	ft_putnbr((int) file->file_stat.st_size);
	ft_putchar('\t');
	return 0;
}

static char print_file_name(file_data *file) {
	ft_putstr(get_after_last_sep(file->path, '/'));
	return 0;
}

static char print_file_details(file_data *file) {
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

	for (int i = LS_PRINT_TYPE; i <= LS_PRINT_LINK; i++)
		print[i](file);

	ft_putchar('\n');
	return 0;
}

char print_file_props(char flags, ft_list *file_ptr) {
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

char print_file_props_no_list(file_data *file, char flags) {
	if (!file)
		return -1;

	if (LS_HAS_FLAG_l(flags))
		return print_file_details(file);
	print_file_name(file);
	return 0;
}

void print_no_such_file(char *name) {
	ft_fputstr("ft_ls: cannot access '", 2);
	ft_fputstr(name, 2);
	ft_fputstr("': No such file or directory\n", 2);
}
