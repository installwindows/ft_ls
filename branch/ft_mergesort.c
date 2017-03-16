/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mergesort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/14 18:58:46 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/14 21:49:47 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ft_ls.h"
#include "libft.h"
*/
void	add(t_file **list, const char *data)
{
	t_file	*new;

	new = malloc(sizeof(t_file));
	new->e = malloc(sizeof(struct dirent));
	strcpy(new->e->d_name, data);
	new->next = *list;
	*list = new;
}

/*
void	print(t_file *list)
{
	while (list)
	{
		printf("%3s, ", list->e->d_name);
		list = list->next;
	}
	printf("\n");
}
*/

void	split(t_file *list, t_file **a, t_file **b)
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

t_file	*merge(t_file *a, t_file *b, int (*cmp)(void *, void *))
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

void	ft_mergesort(t_file **list, int (*cmp)(void *, void *))
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

int		cmp_alpha(void *a, void *b)
{
	t_file	*x;
	t_file	*y;

	x = (t_file*)a;
	y = (t_file*)b;
	return (ft_strcmp(x->e->d_name, y->e->d_name));
}

int		cmp_revalpha(void *a, void *b)
{
	t_file	*x;
	t_file	*y;

	x = (t_file*)a;
	y = (t_file*)b;
	return (ft_strcmp(x->e->d_name, y->e->d_name) < 0);
}

int		cmp_time(void *a, void *b)
{
	t_file	*x;
	t_file	*y;

	x = (t_file*)a;
	y = (t_file*)b;
	return (GETTIME(x->s) < GETTIME(y->s));
}

int		cmp_revtime(void *a, void *b)
{
	t_file	*x;
	t_file	*y;

	x = (t_file*)a;
	y = (t_file*)b;
	return (GETTIME(x->s) > GETTIME(y->s));
}
/*
int		main(void)
{
	t_file	*list;

	list = NULL;
	add(&list, "5");
	add(&list, "6");
	add(&list, "9");
	add(&list, "18");
	add(&list, "0");
	add(&list, "77");
	add(&list, "66");
	add(&list, "1");
	add(&list, "167");
	add(&list, "4");
	add(&list, "87");
	add(&list, "87");
	print(list);
	ft_mergesort(&list, cmp_revalpha);
	print(list);
}
*/
