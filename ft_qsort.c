/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_qsort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/28 02:22:14 by varnaud           #+#    #+#             */
/*   Updated: 2017/02/28 03:01:15 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

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
		*x = *y;
		*y = tmp;
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
		l = begin + size;
		r = end;
		while (l < r)
		{
			if (cmp(array + l, pivot) <= 0)
				l += size;
			else if (cmp(array + r, pivot) > 0)
				r -= size;
			else if (l < r)
				swap(array + l, array + r, size);
		}
		l -= size;
		swap(array + begin, array + l, size);
		sort(array, size, cmp, begin, l);
		sort(array, size, cmp, r, end);
	}
}

void		ft_qsort(void *array, int n, int size,
					int (*cmp)(const void *, const void *))
{
	sort(array, size, cmp, 0, n * size);
}

int			cmpstr(const void *s1, const void *s2)
{
	const char	*a;
	const char	*b;

	a = s1;
	b = s2;
	while (*a == *b && *a && *b)
	{
		a++;
		b++;
	}
	return (*a - *b);
}

int			main(int argc, char **argv)
{
	if (argc > 1)
	{
		ft_qsort(++argv, argc - 1, sizeof(char *), cmpstr);
		while (--argc)
			printf("%s\n", *argv++);
	}
}
