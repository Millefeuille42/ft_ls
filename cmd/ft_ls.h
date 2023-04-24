//
// Created by millefeuille on 4/24/23.
//

#ifndef FT_LS_FT_LS_H
# define FT_LS_FT_LS_H

# include <dirent.h>
# include <sys/stat.h>
# include <pwd.h>
# include <grp.h>
# include <time.h>
# include <string.h>

# include "../pkg/ft_error/ft_error.h"
# include "../pkg/ft_list/ft_list.h"
# include "../pkg/ft_string/ft_string.h"

# define LS_FLAG_l 1  	// 000001
# define LS_FLAG_a 2  	// 000010
# define LS_FLAG_r 4  	// 000100
# define LS_FLAG_R 8  	// 001000
# define LS_FLAG_t 16 	// 010000
# define LS_FLAG_dirs 32 // 100000 Should have multi directory display ?

# define LS_SET_FLAG_l(flags) (flags |= LS_FLAG_l)
# define LS_SET_FLAG_a(flags) (flags |= LS_FLAG_a)
# define LS_SET_FLAG_r(flags) (flags |= LS_FLAG_r)
# define LS_SET_FLAG_R(flags) (flags |= LS_FLAG_R)
# define LS_SET_FLAG_t(flags) (flags |= LS_FLAG_t)
# define LS_SET_FLAG_dirs(flags) (flags |= LS_FLAG_dirs)

# define LS_HAS_FLAG_l(flags) (flags & LS_FLAG_l)
# define LS_HAS_FLAG_a(flags) (flags & LS_FLAG_a)
# define LS_HAS_FLAG_r(flags) (flags & LS_FLAG_r)
# define LS_HAS_FLAG_R(flags) (flags & LS_FLAG_R)
# define LS_HAS_FLAG_t(flags) (flags & LS_FLAG_t)
# define LS_HAS_FLAG_dirs(flags) (flags & LS_FLAG_dirs)

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

file_data *get_data_from_file(char *name, char *path);
size_t get_total_blocks(ft_list *file_list);
ls_args parse_args(int argc, char **argv);

char print_file_props(char flags, ft_list *file_ptr);
char print_file_props_no_list(file_data *file, char flags);
void print_no_such_file(char *name);

char file_name_alpha_sort(void *a, void *b);
char file_ctime_desc_sort(void *a, void *b);

void del_file_data(void **ptr);
int has_trailing_slash(char *path);
int is_cur_dir_or_prev_dir(char *dir_name);

#endif //FT_LS_FT_LS_H
