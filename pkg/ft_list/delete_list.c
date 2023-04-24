//
// Created by millefeuille on 4/21/23.
//

#include "ft_list.h"

void delete_element(ft_list *element, list_data_del_func del) {
	if (!element)
		return;

	if (element->prev)
		element->prev->next = element->next;
	if (element->next)
		element->next->prev = element->prev;

	if (del)
		del(&element->data);
	safe_free((void **) &element);
}

void delete_list_forward(ft_list **list, list_data_del_func del) {
	if (!list || !*list)
		return;

	ft_list *current = *list;
	ft_list *next = NULL;
	for (; current; current = next) {
		next = current->next;
		delete_element(current, del);
	}
	*list = NULL;
}

void delete_list_backward(ft_list **list, list_data_del_func del) {
	if (!list || !*list)
		return;

	ft_list *current = *list;
	ft_list *next = NULL;
	for (; current; current = next) {
		next = current->next;
		delete_element(current, del);
	}
	*list = NULL;
}
