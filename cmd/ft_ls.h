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

# define LS_FLAG_l		1   		// 00000000 00000001
# define LS_FLAG_a		2  			// 00000000 00000010
# define LS_FLAG_r		4  			// 00000000 00000100
# define LS_FLAG_R		8  			// 00000000 00001000
# define LS_FLAG_t		16 			// 00000000 00010000
# define LS_FLAG_G		32			// 00000000 00100000
# define LS_FLAG_U		128			// 00000000 10000000
# define LS_FLAG_f		256			// 00000001 00000000
# define LS_FLAG_g		512			// 00000010 00000000
# define LS_FLAG_A		1024		// 00000100 00000000
# define LS_FLAG_Q		2048		// 00001000 00000000
# define LS_FLAG_o		4096		// 00010000 00000000
# define LS_FLAG_p		8192		// 00100000 00000000
# define LS_FLAG_dirs 	16384		// 01000000 00000000 Should have multi directory display ?

# define LS_SET_FLAG(flags, flag) (flags |= flag)
# define LS_CLEAR_FLAG(flags, flag) (flags &= ~(flag))
# define LS_HAS_FLAG(flags, flag) (flags & flag)

typedef struct s_ls_args {
	ft_list *directories;
	short flags;
	char err;
} ls_args;

typedef struct s_file_data {
	struct dirent *file_info;
	char *path;
	struct stat file_stat;
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

typedef char (*dir_func)(short flags, ft_list *file_ptr);
typedef char (*file_print_func)(file_data *file, short flags);

file_data *get_data_from_file(char *name, char *path);
size_t get_total_blocks(ft_list *file_list);
ls_args parse_args(int argc, char **argv);

char print_file_props(short flags, ft_list *file_ptr);
char print_file_props_no_list(file_data *file, short flags);
void print_no_such_file(char *name);

char file_name_alpha_sort(void *a, void *b);
char file_ctime_desc_sort(void *a, void *b);

void del_file_data(void **ptr);
int has_trailing_slash(char *path);
int is_cur_dir_or_prev_dir(char *dir_name);

#endif //FT_LS_FT_LS_H
