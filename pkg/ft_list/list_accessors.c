//
// Created by millefeuille on 4/21/23.
//

#include "ft_list.h"

ft_list *start_of_list(ft_list *list) {
	if (!list)
		return NULL;

	ft_list *current = list;
	for (; current->prev; current = current->prev);
	return current;
}

ft_list *end_of_list(ft_list *list) {
	if (!list)
		return NULL;

	ft_list *current = list;
	for (; current->next; current = current->next);
	return current;
}
