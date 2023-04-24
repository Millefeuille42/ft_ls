//
// Created by millefeuille on 4/24/23.
//

#include "ft_ls.h"

static char parse_flags(char flags, char *str) {
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
				LS_SET_FLAG_dirs(flags);
				break;
			case 't':
				LS_SET_FLAG_t(flags);
				break;
			default:
				if (str[i2]) {
					ft_putstr("Invalid flag: ");
					ft_putchar(str[i2]);
					ft_putchar('\n');
					return -1;
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
		if (argv[i][0] == '-' && ft_strlen(argv[i]) > 1) {
			ret.flags = parse_flags(ret.flags, argv[i]);
			if (ret.flags < 0) {
				ret.err = -2;
				delete_list_forward(&directories, safe_free);
				return ret;
			}
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
