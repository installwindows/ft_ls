/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/06 14:49:16 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/06 14:54:28 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>

void	*ft_memdup(const void *source, size_t size)
{
	unsigned char	*dup;
	size_t			i;

	dup = malloc(size);
	i = 0;
	while (i < size)
	{
		dup[i] = ((unsigned char*)source)[i];
		i++;
	}
	return (dup);
}
/*
int		main(void)
{
	const char *str = "Hello";
	char		*dup;

	dup = ft_memdup(str, 6);
	printf("%s\n", dup);
}
*/
