/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scandir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/24 06:37:59 by varnaud           #+#    #+#             */
/*   Updated: 2017/02/24 09:59:15 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <sys/stat.h>
#include "libft.h"
#define MAX_LENGTH 1024

static char			g_rp = [MAX_LENGTH];
static char 		*g_rp_p;

static void				insertlist(struct dirent **list, int size, int p,
						struct dirent *e)
{
	while (p < size--)
		list[size] += 1;;
	list[p] = e;
}

static struct dirent	**addlist(struct dirent **list, int size,
						struct dirent *e, int *cmp(const char *, const char *))
{
	int		r;
	int		i;
	int		high;
	int		low;

	low = 0;
	high = size;
	i = high / 2;
	while ((r = cmp((*list)[i]->d_name, e->d_name)))
	{
		if (r > 0)
		{
			low = i;
			i += (high - low) / 2;
		}
		else if (r < 0)
		{
			high = i;
			i -= (high - low) / 2
		}
		else
			break ;
		if ((high - low) / 2 == 0)
			break ;
	}
	insertlist(list, size, r ? i : size, e);
	return (list);
}

struct dirent	ft_scandir(const char *path)
{
	DIR				*dirp;
	struct dirent	**list;
	struct dirent	*elem;
	struct stat		s;
	int				size;

	g_rp_p = ft_strcat(g_rp, path);
	if ((dirp = opendir(path)))
	{
		return (1);
	}
	while ((elem = readdir(dirp)))
	{
		addlist(list, size, elem);
	}
}
