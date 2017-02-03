/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/28 02:46:40 by varnaud           #+#    #+#             */
/*   Updated: 2017/02/03 11:26:17 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <sys/stat.h>
#include "ft_ls.h"

int		ft_ls(char *path)
{
	DIR				*dirp;
	struct dirent	*cur;
	t_dir			*file_list;
	t_dir			*dir_list;
	t_dir			*current;

	file_list = NULL;
	dir_list = NULL;
	current = file_list;
	dirp = opendir(path);
	if (dirp)
	{
		while ((cur = readdir(dirp)))
		{
			current->file = cur;
			current->next = NULL;
			current = current->next;
		}
	}
	else
	{
		
	}
}
