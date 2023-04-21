//
// Created by millefeuille on 4/21/23.
//

#include "ft_list.h"

void iter_list_forward(ft_list *list, list_func func) {
	if (!list)
		return;

	ft_list *current = list;
	ft_list *next = NULL;
	for (; current; current = next) {
		next = current->next;
		func(current);
	}
}

void iter_list_backward(ft_list *list, list_func func) {
	if (!list)
		return;

	ft_list *current = list;
	ft_list *prev = NULL;
	for (; current; current = prev) {
		prev = current->prev;
		func(current);
	}
}
