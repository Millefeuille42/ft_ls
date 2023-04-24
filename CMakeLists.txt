cmake_minimum_required(VERSION 3.10)
project(ft_ls C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall -Werror -Wextra -pedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Werror -Wextra -pedantic")

set(SOURCE_FT_LS
		cmd/main.c
		cmd/prints.c
		cmd/parsing.c
		cmd/sorts.c
		cmd/utils.c
		cmd/fs_helpers.c
		cmd/ft_ls.h

		pkg/ft_print/ft_putstr.c
		pkg/ft_print/ft_putchar.c
		pkg/ft_print/ft_putnbr.c
		pkg/ft_print/ft_print.h

		pkg/ft_string/ft_strlen.c
		pkg/ft_string/string.c
		pkg/ft_string/ft_string_concat.c
		pkg/ft_string/ft_string_copy.c
		pkg/ft_string/ft_split.c
		pkg/ft_string/get_after_n_sep.c
		pkg/ft_string/ft_to_lower.c
		pkg/ft_string/ft_string.h

		pkg/ft_memory/safe_free.c
		pkg/ft_memory/del_array.c
		pkg/ft_memory/ft_bzero.c
		pkg/ft_memory/zeroed_malloc.c
		pkg/ft_memory/ft_memory.h

		pkg/ft_error/panic.c
		pkg/ft_error/log_error.c
		pkg/ft_error/ft_error.h

		pkg/ft_list/ft_list.c
		pkg/ft_list/iter_list.c
		pkg/ft_list/delete_list.c
		pkg/ft_list/list_accessors.c
		pkg/ft_list/list_sort.c
		pkg/ft_list/ft_list.h
		)

add_executable(ft_ls ${SOURCE_FT_LS})
target_link_libraries(ft_ls LINK_PUBLIC)