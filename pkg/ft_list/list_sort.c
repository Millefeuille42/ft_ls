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
	for (ft_list *smallest = get_smallest(list, comparator, reverse); smallest; smallest = get_smallest(list, comparator, reverse)) {
		ft_list *element = new_element_to_list(new_list, smallest->data);
		if (!element) {
			delete_list_forward(&element, NULL);
			return NULL;
		}
		if (!new_list) {
			new_list = element;
		}
		if (smallest == list)
			list = smallest->next;
		delete_element(smallest, NULL);
	}

	return new_list;
}
