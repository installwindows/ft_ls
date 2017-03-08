/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nls.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/07 13:19:31 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/07 16:30:09 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include "tmp.h"
#include "ft_ls.h"
#include "libft.h"
#include "ft_printf.h"

void	getstat(char *path, char *name, struct stat *s, t_dir **current)
{
	char	*pathname;
	t_dir	*dir;

	pathname = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(name) + 1));
	ft_strcat(pathname, path);
	ft_strcat(pathname, "/");
	ft_strcat(pathname, name);
	stat(pathname, s);
	if (S_ISDIR(s->st_mode) && name[0] != '.')
	{
		dir = malloc(sizeof(t_dir));
		dir->path = pathname;
		dir->next = NULL;
		if (*current == NULL)
			*current = dir;
		else
		{
			(*current)->next = dir;
			current = &dir;
		}
	}
	else
		;//free(pathname);
}

int		ft_listdir(char *path)
{
	struct dirent	**list;
	struct stat		s;
	int				size;
	t_dir			*dirlist;
	t_dir			**current;

	dirlist = NULL;
	current = &dirlist;
	size = ft_scandir(path, &list, NULL, dir_alphasort);
	if (size < 0)
		return (-1);
	while (size--)
	{
		getstat(path, (*list)->d_name, &s, current);
		ft_printf("%s\n", (*list)->d_name);
		free(*list);
		list++;
	}
	//free(list);
	while (dirlist)
	{
		ft_printf("\n%s:\n", dirlist->path);
		ft_listdir(dirlist->path);
		dirlist = dirlist->next;
		if (dirlist)
			;//ft_printf("\n");
	}
	return (0);
}

int		ft_ls(char *dirname)
{
	ft_listdir(dirname);
	return (0);
}
