//
// Created by millefeuille on 4/21/23.
//

#include "ft_list.h"

ft_list *new_list_element(void *data) {
	ft_list *ret = zeroed_malloc(sizeof(ft_list));
	if (!ret)
		return NULL;

	ret->data = data;
	return ret;
}

void attach_element_to_list(ft_list *list, ft_list *element) {
	if (!list || !element)
		return;

	ft_list *end = end_of_list(list);
	end->next = element;
	element->prev = end;
}

void remove_element_from_list(ft_list *element) {
	if (!element)
		return;

	if (element->prev)
		element->prev->next = element->next;
	if (element->next)
		element->next->prev = element->prev;

	element->prev = NULL;
	element->next = NULL;
}

ft_list *new_element_to_list(ft_list *list, void *data) {
	if (!list)
		return new_list_element(data);

	ft_list *new_element = new_list_element(data);
	if (!new_element)
		return NULL;

	attach_element_to_list(list, new_element);
	return new_element;
}
