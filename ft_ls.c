/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/28 02:46:40 by varnaud           #+#    #+#             */
/*   Updated: 2017/02/01 17:15:45 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <sys/stat.h>
#include "ft_ls.h"

int		list_dir(char *dir_name)
{
	DIR				*dirp;
	struct dirent	*cur;
	t_dir			*file_list;
	t_dir			*dir_list;
	t_dir			*current;

	file_list = NULL;
	dir_list = NULL;
	current = file_list;
	dir = opendir(dir_name);
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

int		ft_ls(void)
{
	struct stat	file_stat;

	while (*(g_opt->p_files))
	{
		stat(*(g_opt->p_files), &file_stat);
		if (S_ISREG(file_stat.st_mode))
			list_dir(*(g_opt->p_files));
	}
}
