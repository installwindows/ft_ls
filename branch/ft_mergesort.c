/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mergesort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/14 18:58:46 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/14 19:39:12 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

typedef struct		s_node
{
	int				data;
	struct s_node	*next;
}					t_node;

void	add(t_node **list, int data)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	new->data = data;
	new->next = *list;
	*list = new;
}

void	print(t_node *list)
{
	while (list)
	{
		printf("%3d, ", list->data);
		list = list->next;
	}
	printf("\n");
}

void	split(t_node *list, t_node **a, t_node **b)
{
	t_node	*fast;
	t_node	*slow;

	if (list == NULL || list->next == NULL)
	{
		*a = list;
		*b = NULL;
		return ;
	}
	slow = list;
	fast = list->next;
	while (fast)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*a = list;
	*b = slow->next;
	slow->next = NULL;
}

t_node	*merge(t_node *a, t_node *b)
{
	t_node	*result;

	result = NULL;
	if (a == NULL)
		return (b);
	if (b == NULL)
		return (a);
	if (a->data <= b->data)
	{
		result = a;
		result->next = merge(a->next, b);
	}
	else
	{
		result = b;
		result->next = merge(a, b->next);
	}
	return (result);
}

void	ft_mergesort(t_node **list)
{
	t_node	*a;
	t_node	*b;

	if (*list == NULL || (*list)->next == NULL)
		return ;
	split(*list, &a, &b);
	ft_mergesort(&a);
	ft_mergesort(&b);
	*list = merge(a, b);
}

int		main(void)
{
	t_node	*list;

	list = NULL;
	add(&list, 5);
	add(&list, 6);
	add(&list, 9);
	add(&list, 18);
	add(&list, 0);
	add(&list, 77);
	add(&list, 66);
	add(&list, 1);
	add(&list, 167);
	add(&list, 4);
	add(&list, 87);
	add(&list, 87);
	print(list);
	ft_mergesort(&list);
	print(list);
}
