######### Sources #########

SOURCES	= 		cmd/main.c \
           \
          		pkg/ft_print/ft_putstr.c \
          		pkg/ft_print/ft_putchar.c \
          		pkg/ft_print/ft_putnbr.c \
           \
          		pkg/ft_string/ft_strlen.c \
          		pkg/ft_string/string.c \
          		pkg/ft_string/ft_string_concat.c \
          		pkg/ft_string/ft_string_copy.c \
          		pkg/ft_string/ft_split.c \
          		pkg/ft_string/get_after_n_sep.c \
          		pkg/ft_string/ft_to_lower.c \
           \
          		pkg/ft_memory/safe_free.c \
          		pkg/ft_memory/ft_bzero.c \
          		pkg/ft_memory/zeroed_malloc.c \
          		pkg/ft_memory/del_array.c \
           \
          		pkg/ft_error/panic.c \
          		pkg/ft_error/log_error.c \
           \
          		pkg/ft_list/ft_list.c \
          		pkg/ft_list/iter_list.c \
          		pkg/ft_list/delete_list.c \
          		pkg/ft_list/list_accessors.c \
          		pkg/ft_list/list_accessors.c \
          		pkg/ft_list/list_sort.c \
          \
          		cmd/prints.c \
                cmd/parsing.c \
                cmd/sorts.c \
                cmd/utils.c \
                cmd/fs_helpers.c


HEADERS	=		cmd/ft_ls.h \
			\
				pkg/ft_print/ft_print.h \
          		pkg/ft_string/ft_string.h \
          		pkg/ft_error/ft_error.h \
          		pkg/ft_list/ft_list.h \
          		pkg/ft_memory/ft_memory.h

HEADERS_DIRECTORIES	=			cmd \
								pkg/ft_print/ \
                                pkg/ft_string/ \
                                pkg/ft_error/ \
                                pkg/ft_list/ \
                                pkg/ft_memory/

######### Details #########

NAME	=	ft_ls
SOURCES_EXTENSION = c

######### Compilation #########

COMPILE		=	clang
DELETE		=	rm -f

FLAGS		=	-Wall -Werror -Wextra -pedantic -Ofast

######### Additional Paths #########

vpath	%.h $(HEADERS_DIRECTORIES)

# ################################### #
#        DO NOT ALTER FURTHER!        #
# ################################### #

######### Additional Paths #########

vpath	%.o $(OBJS_DIR)
vpath	%.d $(DEPS_DIR)

######### Implicit Macros #########

OBJS_DIR	= .objs/
DEPS_DIR	= .deps/

OBJS	=	$(addprefix $(OBJS_DIR), $(SOURCES:.$(SOURCES_EXTENSION)=.o))
DEPS	=	$(addprefix $(DEPS_DIR), $(SOURCES:.$(SOURCES_EXTENSION)=.d))

#########  Rules  #########

all:	$(OBJS_DIR) $(DEPS_DIR) $(NAME) ## Compile project and dependencies

$(NAME):	$(OBJS) ## Compile project
			$(COMPILE) $(FLAGS) $^ -o $@

clean: clean_deps clean_objs ## Delete object files

fclean:	clean clean_bin ## Delete object files and binary

re:	fclean ## Delete object files and binary, then recompile all
			@make --no-print-directory all

help: ## Print this help
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z_-]+:.*?## / {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}' $(MAKEFILE_LIST)

#########  Sub Rules  #########

objs: $(OBJS_DIR) $(DEPS_DIR) $(OBJS) ## Compile object files

clean_deps: ## Delete dependency files and directory
			$(DELETE) -r $(DEPS_DIR)
clean_objs: ## Delete object files and directory
			$(DELETE) -r $(OBJS_DIR)
clean_bin: ## Delete produced binary
			$(DELETE) $(NAME)

#########  Implicit Rules  #########

$(OBJS_DIR):
					@mkdir -p $(OBJS_DIR)

$(DEPS_DIR):
					@mkdir -p $(DEPS_DIR)

$(OBJS_DIR)%.o:	%.$(SOURCES_EXTENSION)
			mkdir -p $(OBJS_DIR)$(dir $<)
			@mkdir -p $(DEPS_DIR)$(dir $<)
			$(COMPILE) $(FLAGS) -MMD -MP -MF $(DEPS_DIR)$*.d -c $< -o $@

.PHONY	:	all clean fclean re help

#########  Includes  #########

-include $(DEPS)
