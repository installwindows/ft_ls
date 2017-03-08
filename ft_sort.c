/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/06 16:41:06 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/06 16:58:23 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_lexisort(const char *s1, const char *s2)
{
	unsigned char	a;
	unsigned char	b;

	while (*s1 == *s2 && *s1 && s2)
	{
		s1++;
		s2++;
	}
	a = (unsigned char)*s1;
	b = (unsigned char)*s2;
	if (a > b)
		return (b - a);
	if (a < b)
		return (a - b);
	return (0);
}
