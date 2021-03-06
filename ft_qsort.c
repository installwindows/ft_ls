/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_qsort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/28 02:22:14 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/09 00:04:26 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include "libft.h"

static void	swap(void *a, void *b, size_t size)
{
	char	*x;
	char	*y;
	char	tmp;

	x = a;
	y = b;
	while (size--)
	{
		tmp = *x;
		*x++ = *y;
		*y++ = tmp;
	}
}

static void	sort(void *array, size_t size,
			int (*cmp)(const void *, const void *), int begin, int end)
{
	void	*pivot;
	int		l;
	int		r;

	if (end > begin)
	{
		pivot = array + begin;
		//swap(array + begin, pivot, size);
		l = begin;
		r = end;
		while (l < r)
		{
			while (l < end && cmp(array + l, pivot) < 0)
				l += size;
			while (cmp(array + r, pivot) > 0)
				r -= size;
			if (l < r)
			{
				swap(array + l, array + r, size);
				l += size;
				r -= size;
			}
		}
		//if (r >= 0)
			//swap(pivot, array + r, size);
		sort(array, size, cmp, begin, r);
		sort(array, size, cmp, r, end);
	}
}

void		ft_qsort(void *array, int n, int size,
					int (*cmp)(const void *, const void *))
{
	//sort(array, size, cmp, 0, (n - 1) * size);
	//I've broken it, so defaulting to bubble sort for now
	int		i;
	int		j;

	i = 0;
	while (i < n)
	{
		j = i;
		while (j < n)
		{
			if (cmp(array + (i * size), array + (j * size)) > 0)
				swap(array + (i * size), array + (j * size), size);
			j++;
		}
		i++;
	}
}

int			cmpstr(const void *s1, const void *s2)
{
	const char	*a;
	const char	*b;

	a = *(char**)s1;
	b = *(char**)s2;
	return (ft_strcmp(a, b));
}

/*
int			main(int argc, char **argv)
{
	if (argc > 1)
	{
		ft_qsort(++argv, argc - 1, sizeof(char *), cmpstr);
		while (--argc)
			printf("%s\n", *argv++);
	}
}
*/
