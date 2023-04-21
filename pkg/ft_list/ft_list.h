//
// Created by millefeuille on 4/21/23.
//

#ifndef FT_LS_FT_LIST_H
# define FT_LS_FT_LIST_H

#include "../ft_memory/ft_memory.h"

typedef struct s_ft_list {
	void *data;
	struct s_ft_list *next;
	struct s_ft_list *prev;
} ft_list;

typedef void (*list_func)(ft_list *);
typedef void (*list_data_del_func)(void **);

ft_list *new_list_element(void *data);
void attach_element_to_list(ft_list *list, ft_list *element);
ft_list *new_element_to_list(ft_list *list, void *data);

void iter_list_forward(ft_list *list, list_func func);
void iter_list_backward(ft_list *list, list_func func);

ft_list *start_of_list(ft_list *list);
ft_list *end_of_list(ft_list *list);

void delete_element(ft_list *element, list_data_del_func del);
void delete_list_forward(ft_list **list, list_data_del_func del);
void delete_list_backward(ft_list **list, list_data_del_func del);

#endif //FT_LS_FT_LIST_H
