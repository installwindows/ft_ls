/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/18 23:49:40 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/19 00:05:11 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "ft_ls.h"

static void	free_file(t_file *file)
{
	if (file->e)
		free(file->e);
	if (file->path)
		free(file->path);
	//if (file->pw)
	//	free(file->pw);
	//if (file->gr)
	//	free(file->gr);
	free(file);
}

void		free_dir(t_dir *dir)
{
	t_file	*f;
	t_file	*t;

	f = dir->list;
	while (f)
	{
		t = f;
		f = dir->list->next;
		free_file(t);
	}
	f = dir->dirlist;
	while (f)
	{
		t = f;
		f = dir->dirlist->next;
		free(t);
	}
	free(dir);
}
