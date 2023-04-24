//
// Created by millefeuille on 4/24/23.
//

#include "ft_list.h"

static ft_list *get_smallest(ft_list *list, list_sort_func comparator, char reverse) {
	ft_list *current = list;
	ft_list *smallest = current;
	for (; current; current = current->next) {
		if ((!reverse && comparator(smallest->data, current->data))
			|| (reverse && comparator(current->data, smallest->data)))
			continue;
		smallest = current;
	}

	return smallest;
}
ft_list *list_sort(ft_list *list, list_sort_func comparator, char reverse) {

	if (!list)
		return NULL;

	ft_list *new_list = NULL;
	ft_list *new_list_cursor = NULL;
	for (ft_list *smallest = get_smallest(list, comparator, reverse); smallest; smallest = get_smallest(list, comparator, reverse)) {
		if (smallest == list)
			list = smallest->next;
		remove_element_from_list(smallest);
		if (!new_list) {
			new_list = smallest;
			new_list_cursor = new_list;
			continue;
		}
		new_list_cursor->next = smallest;
		smallest->prev = new_list_cursor;
		new_list_cursor = smallest;
	}

	return new_list;
}
