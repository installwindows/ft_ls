/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mergesort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/14 18:58:46 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/18 22:41:01 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_ls.h"
#include "libft.h"

static void		split(t_file *list, t_file **a, t_file **b)
{
	t_file	*fast;
	t_file	*slow;

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

static t_file	*merge(t_file *a, t_file *b, int (*cmp)(void *, void *))
{
	t_file	*result;

	result = NULL;
	if (a == NULL)
		return (b);
	if (b == NULL)
		return (a);
	if (cmp(a, b) <= 0)
	{
		result = a;
		result->next = merge(a->next, b, cmp);
	}
	else
	{
		result = b;
		result->next = merge(a, b->next, cmp);
	}
	return (result);
}

void			ft_mergesort(t_file **list, int (*cmp)(void *, void *))
{
	t_file	*a;
	t_file	*b;

	if (*list == NULL || (*list)->next == NULL)
		return ;
	split(*list, &a, &b);
	ft_mergesort(&a, cmp);
	ft_mergesort(&b, cmp);
	*list = merge(a, b, cmp);
}
