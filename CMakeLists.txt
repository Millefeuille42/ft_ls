cmake_minimum_required(VERSION 3.10)
project(ft_ls C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall -Werror -Wextra -pedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Werror -Wextra -pedantic")

set(SOURCE_FT_LS

		cmd/main.c)

add_executable(ft_ls ${SOURCE_FT_LS})
target_link_libraries(ft_ls LINK_PUBLIC)