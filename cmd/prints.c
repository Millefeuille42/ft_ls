//
// Created by millefeuille on 4/24/23.
//

#include "ft_ls.h"

static char print_file_permissions(file_data *file, short flags) {
	(void)flags;
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

static char print_file_type(file_data *file, short flags) {
	(void)flags;
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

static char print_file_owner_user(file_data *file, short flags) {
	if (LS_HAS_FLAG(flags, LS_FLAG_g))
		return 0;
	struct passwd *p_uid = getpwuid(file->file_stat.st_uid);
	if (!p_uid)
		return -1;
	ft_putstr(p_uid->pw_name);
	ft_putchar('\t');
	return 0;
}

static char print_file_owner_group(file_data *file, short flags) {
	if (LS_HAS_FLAG(flags, LS_FLAG_G))
		return 0;
	struct group *p_gid = getgrgid(file->file_stat.st_gid);
	if (!p_gid)
		return -1;
	ft_putstr(p_gid->gr_name);
	ft_putchar('\t');
	return 0;
}

static char print_file_last_edit(file_data *file, short flags) {
	(void)flags;
	time_t current_time;
	time(&current_time);
	char *t_str = ctime(&file->file_stat.st_ctim.tv_sec);
	if (!t_str)
		return -1;
	char **t_arr = ft_split(t_str, ' ');
	if (!t_arr)
		return -1;
	char *current_year = get_after_n_sep(ctime(&current_time), ' ', 4) ;
	if (!current_year)
		return -1;

	for (size_t i = 1; t_arr[i] && i < 4; i++) {
		if (i == 3) {
			if (!ft_strcmp(current_year, t_arr[4])) {
				t_arr[i][5] = '\0';
				ft_putstr(t_arr[i]);
			}
			else {
				t_arr[4][ft_strlen(t_arr[4]) - 1] = '\0';
				ft_putstr(t_arr[4]);
			}
			continue;
		}
		ft_putstr(t_arr[i]);
		ft_putchar(' ');
	}

	del_array((void **) &t_arr);
	ft_putchar('\t');
	return 0;
}

static char print_file_link(file_data *file, short flags) {
	(void)flags;
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

static char print_file_hardlink(file_data *file, short flags) {
	(void)flags;
	ft_putnbr((int) file->file_stat.st_nlink);
	ft_putchar('\t');
	return 0;
}

static char print_file_size(file_data *file, short flags) {
	(void)flags;
	ft_putnbr((int) file->file_stat.st_size);
	ft_putchar('\t');
	return 0;
}

static char print_file_name(file_data *file, short flags) {
	(void)flags;
	ft_putstr(get_after_last_sep(file->path, '/'));
	return 0;
}

static char print_file_details(file_data *file, short flags) {
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
		print[i](file, flags);

	ft_putchar('\n');
	return 0;
}

char print_file_props(short flags, ft_list *file_ptr) {
	if (!file_ptr)
		return -1;

	file_data *file = file_ptr->data;

	if (LS_HAS_FLAG(flags, LS_FLAG_l))
		return print_file_details(file, flags);

	print_file_name(file, flags);
	if (file_ptr->next)
		ft_putchar(' ');
	return 0;
}

char print_file_props_no_list(file_data *file, short flags) {
	if (!file)
		return -1;

	if (LS_HAS_FLAG(flags, LS_FLAG_l))
		return print_file_details(file, flags);
	print_file_name(file, flags);
	return 0;
}

void print_no_such_file(char *name) {
	ft_fputstr("ft_ls: cannot access '", 2);
	ft_fputstr(name, 2);
	ft_fputstr("': No such file or directory\n", 2);
}

void print_help(void) {
	/* OK */ ft_putstr("Usage: ft_ls [OPTION]... [FILE]...\n");
	/* OK */ ft_putstr("List information about the FILEs (the current directory by default).\n");
	/* OK */ ft_putstr("Sort entries alphabetically if none of -ftU is specified.\n\n");
	/* OK */ ft_putstr("-l\t\tuse a long listing format\n");
	/* OK */ ft_putstr("-a\t\tdo not ignore entries starting with .\n");
	/* OK */ ft_putstr("-r\t\treverse order while sorting\n");
	/* OK */ ft_putstr("-R\t\tlist subdirectories recursively\n");
	/* OK */ ft_putstr("-t\t\tsort by time, newest first; see --time\n");
	/* OK */ ft_putstr("-G\t\tin a long listing, don't print group names\n");
	/* OK */ ft_putstr("-U\t\tdo not sort; list entries in directory order\n");
	/* OK */ ft_putstr("-f\t\tdo not sort, enable -aU, disable -ls --color\n");
	/* OK */ ft_putstr("-g\t\tlike -l, but do not list owner\n");
	/* OK */ ft_putstr("-A\t\tdo not list implied . and ..\n");
	ft_putstr("-Q\t\tenclose entry names in double quotes\n");
	ft_putstr("-o\t\tlike -l, but do not list group information\n");
	ft_putstr("-p\t\tappend / indicator to directories\n");
	/* OK */ ft_putstr("--help\tdisplay this help and exit");
}
