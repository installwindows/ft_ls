/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rand.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/05 11:34:16 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/05 12:11:06 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void	*ft_rand(size_t size)
{
	unsigned char	*p;
	int				fd;

	p = malloc(size);
	fd = open("/dev/urandom", O_RDONLY);
	read(fd, p, size);
	close(fd);
	return ((void*)p);
}
