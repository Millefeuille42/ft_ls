//
// Created by millefeuille on 4/24/23.
//

#include "ft_ls.h"

static short parse_flags(short flags, char *str) {
	for (int i = 1; str[i]; i++) {
		switch (str[i]) {
			case 'l':
				LS_SET_FLAG(flags, LS_FLAG_l);
				break;
			case 'a':
				LS_SET_FLAG(flags, LS_FLAG_a);
				LS_CLEAR_FLAG(flags, LS_FLAG_A);
				break;
			case 'r':
				LS_SET_FLAG(flags, LS_FLAG_r);
				break;
			case 't':
				LS_SET_FLAG(flags, LS_FLAG_t);
				LS_CLEAR_FLAG(flags, LS_FLAG_U);
				break;
			case 'U':
				LS_SET_FLAG(flags, LS_FLAG_U);
				break;
			case 'G':
				LS_SET_FLAG(flags, LS_FLAG_G);
				break;
			case 'Q':
				LS_SET_FLAG(flags, LS_FLAG_Q);
			case 'p':
				LS_SET_FLAG(flags, LS_FLAG_p);
			case 'f':
				LS_SET_FLAG(flags, LS_FLAG_f);
				LS_SET_FLAG(flags, LS_FLAG_U);
				LS_SET_FLAG(flags, LS_FLAG_a);
				LS_CLEAR_FLAG(flags, LS_FLAG_l);
				LS_CLEAR_FLAG(flags, LS_FLAG_A);
				break;
			case 'g':
				LS_SET_FLAG(flags, LS_FLAG_g);
				LS_SET_FLAG(flags, LS_FLAG_l);
				break;
			case 'R':
				LS_SET_FLAG(flags, LS_FLAG_R);
				LS_SET_FLAG(flags, LS_FLAG_dirs);
				break;
			case 'A':
				LS_SET_FLAG(flags, LS_FLAG_a);
				LS_SET_FLAG(flags, LS_FLAG_A);
				break;
			case 'o':
				LS_SET_FLAG(flags, LS_FLAG_o);
				LS_SET_FLAG(flags, LS_FLAG_l);
				break;
			default:
				if (str[i]) {
					if (!ft_strcmp(str, "--help")) {
						return -1;
					}
					ft_putstr("Invalid flag: ");
					ft_putchar(str[i]);
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
		LS_SET_FLAG(ret.flags, LS_FLAG_dirs);
	return ret;
}
