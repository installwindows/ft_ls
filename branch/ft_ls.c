/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 19:26:40 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/19 00:12:38 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <sys/xattr.h>
#include "libft.h"
#include "ft_ls.h"
#include "ft_printf.h"

static int		print_dir(t_dir *dir, t_opt *options)
{
	t_file	*c;
	int		bytes;

	bytes = 0;
	c = dir->list;
	if (options->l && dir->list && !options->dirarg)
		bytes += ft_printf("total %d\n", dir->size);
	while (c)
	{
		bytes += print_file(c, options, dir);
		c = c->next;
	}
	return (bytes);
}

static void		check_file(struct dirent *e, t_dir *dir, t_file ***file,
				t_file ***dirlist)
{
	dir->size += (**file)->blocks;
	if ((**file)->nlink > dir->mlink)
		dir->mlink = (**file)->nlink;
	if ((**file)->size > dir->mbyte)
		dir->mbyte = (**file)->size;
	if (ft_strlen((**file)->pw->pw_name) > dir->mpw)
		dir->mpw = ft_strlen((**file)->pw->pw_name);
	if (ft_strlen((**file)->gr->gr_name) > dir->mgr)
		dir->mgr = ft_strlen((**file)->gr->gr_name);
	if (ft_strcmp(e->d_name, ".") &&
		ft_strcmp(e->d_name, "..") && S_ISDIR((**file)->s.st_mode))
	{
		**dirlist = ft_memdup(**file, sizeof(t_file));
		*dirlist = &(**dirlist)->next;
	}
	*file = &(**file)->next;
}

t_dir			*read_dir(const char *dirname, t_opt *options)
{
	struct dirent	*e;
	t_dir			*dir;
	t_file			**dirlist;
	t_file			**current;
	DIR				*pdir;

	current = setup_dir(&dir);
	dirlist = &dir->dirlist;
	if (!(pdir = opendir(dirname)))
	{
		print_error(ft_strchr(dirname, '/') ? ft_strchr(dirname, '/') + 1 :
					dirname);
		return (NULL);
	}
	while ((e = readdir(pdir)))
		if (options->a == 0 && e->d_name[0] == '.')
			continue ;
		else if ((*current = addfile(e, dirname, options)))
			check_file(e, dir, &current, &dirlist);
		else
			options->nberror += print_error(NULL);
	dir->mlink = ft_numlen(dir->mlink);
	dir->mbyte = ft_numlen(dir->mbyte);
	closedir(pdir);
	return (dir);
}

void			sort_lists(t_dir *dir, t_opt *options)
{
	if (!options->f)
	{
		ft_mergesort(&dir->dirlist, options->r && !options->t
									? cmp_revalpha : cmp_alpha);
		if (options->t)
		{
			ft_mergesort(&dir->list, options->r ? cmp_revalpha : cmp_alpha);
			ft_mergesort(&dir->dirlist, options->cmp);
		}
		ft_mergesort(&dir->list, options->cmp);
	}
}

int				ft_ls(t_dir *dir, t_opt *options)
{
	int		bytes;
	int		i;

	if (dir == NULL)
		return (1);
	i = 0;
	sort_lists(dir, options);
	bytes = print_dir(dir, options);
	if (options->cr || options->dirarg)
	{
		options->dirarg = 0;
		while (dir->dirlist)
		{
			if (bytes || i)
				ft_printf("\n%s:\n", dir->dirlist->path);
			else if (options->nbdir > 1)
				ft_printf("%s:\n", dir->dirlist->path);
			else if (options->nberror)
				ft_printf("%s:\n", dir->dirlist->path);
			ft_ls(read_dir(dir->dirlist->path, options), options);
			dir->dirlist = dir->dirlist->next;
			i++;
		}
	}
	//free_dir(dir);
	return (options->nberror);
}
