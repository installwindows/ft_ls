/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/18 22:29:00 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/18 22:30:03 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "ft_ls.h"
#include "libft.h"

int				cmp_alpha(void *a, void *b)
{
	t_file	*x;
	t_file	*y;

	x = (t_file*)a;
	y = (t_file*)b;
	if (x->e && y->e)
		return (ft_strcmp(x->e->d_name, y->e->d_name));
	return (ft_strcmp(x->path, y->path));
}

int				cmp_revalpha(void *a, void *b)
{
	t_file	*x;
	t_file	*y;

	x = (t_file*)a;
	y = (t_file*)b;
	if (x->e && y->e)
		return (ft_strcmp(x->e->d_name, y->e->d_name) < 0);
	return (ft_strcmp(x->path, y->path) < 0);
}

int				cmp_time(void *a, void *b)
{
	t_file	*x;
	t_file	*y;

	x = (t_file*)a;
	y = (t_file*)b;
	if (y->s.st_mtimespec.tv_sec == x->s.st_mtimespec.tv_sec)
		return (y->s.st_mtimespec.tv_nsec - x->s.st_mtimespec.tv_nsec);
	return (y->s.st_mtimespec.tv_sec - x->s.st_mtimespec.tv_sec);
}

int				cmp_revtime(void *a, void *b)
{
	t_file	*x;
	t_file	*y;

	x = (t_file*)a;
	y = (t_file*)b;
	if (y->s.st_mtimespec.tv_sec == x->s.st_mtimespec.tv_sec)
		return (x->s.st_mtimespec.tv_nsec - y->s.st_mtimespec.tv_nsec);
	return (x->s.st_mtimespec.tv_sec - y->s.st_mtimespec.tv_sec);
}
