/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_qsort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/18 19:58:09 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/18 22:09:53 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

int			cmp_alphasort(const void *a, const void *b)
{
	const char	*x;
	const char	*y;

	x = *(char**)a;
	y = *(char**)b;
	return (ft_strcmp(x, y));
}

static void	swap(unsigned char *a, unsigned char *b, size_t size)
{
	unsigned char	tmp;

	while (size--)
	{
		tmp = *a;
		*a++ = *b;
		*b++ = tmp;
	}
}

void		ft_qsort(void *array, int n, int size,
			int (*cmp)(const void *, const void *))
{
	int		i;
	int		j;

	i = 0;
	while (i < n)
	{
		j = i + 1;
		while (j < n)
		{
			if (cmp(array + (i * size), array + (j * size)) > 0)
				swap(array + (i * size), array + (j * size), size);
			j++;
		}
		i++;
	}
}
