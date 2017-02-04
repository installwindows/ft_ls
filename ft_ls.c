/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/28 02:46:40 by varnaud           #+#    #+#             */
/*   Updated: 2017/02/03 17:48:23 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "ft_printf.h"
#include "ft_ls.h"
#include "libft.h"

void	add_sort(t_dir **list, struct dirent *new)
{
	t_dir			*elem;
	static t_dir	*previous;

	if (!list[0] || ft_strcmp((*list)->file->d_name, new->d_name) > 0)
	{
		elem = malloc(sizeof(t_dir));
		elem->file = new;
		if (previous)
			previous->next = elem;
		elem->next = *list;
		*list = elem;
		previous = NULL;
		return ;
	}
	previous = list[0];
	add_sort(&list[1], new);
}

int		ft_ls(char *path)
{
	DIR				*dirp;
	struct dirent	*cur;
	t_dir			*file_list;
	t_dir			*dir_list;

	file_list = NULL;
	dir_list = NULL;
	dirp = opendir(path);
	if (dirp)
	{
		while ((cur = readdir(dirp)))
		{
			if (cur->d_type == DT_DIR)
				add_sort(&dir_list, cur);
			add_sort(&file_list, cur);
		}
		while (file_list)
		{
			ft_printf("%s\n", file_list->file->d_name);
			file_list = file_list->next;
		}
		while (dir_list)
		{
			ft_printf("%s\n", dir_list->file->d_name);
			dir_list = dir_list->next;
		}
	}
	else
	{
		ft_printf("%s\n", path);
	}
	return (0);
}
