/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/16 19:18:22 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/18 22:37:08 by varnaud          ###   ########.fr       */
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

void			usage(char c)
{
	if (c)
	{
		write(2, "ls: illegal option -- ", 22);
		write(2, &c, 1);
		write(2, "\n", 1);
	}
	write(2, "usage: ls [-ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1] [file ...]\n",
																			62);
	exit(1);
}

static t_file	*check_file(t_dir *dir, char **files, t_file ***dlist,
				t_opt *options)
{
	t_file	*new;

	if ((new = addfile(NULL, *files, options)))
	{
		if (S_ISDIR(new->s.st_mode))
		{
			**dlist = new;
			*dlist = &(**dlist)->next;
			options->nbdir++;
			return (NULL);
		}
		if (new->nlink > dir->mlink)
			dir->mlink = new->nlink;
		if (new->size > dir->mbyte)
			dir->mbyte = new->size;
		if (ft_strlen(new->pw->pw_name) > dir->mpw)
			dir->mpw = ft_strlen(new->pw->pw_name);
		if (ft_strlen(new->gr->gr_name) > dir->mgr)
			dir->mgr = ft_strlen(new->gr->gr_name);
		return (new);
	}
	else
		options->nberror += print_error(*files);
	return (NULL);
}

t_dir			*read_files(char **files, t_opt *options)
{
	t_dir		*dir;
	t_file		**dlist;
	t_file		**current;
	t_file		*new;

	current = setup_dir(&dir);
	dlist = &dir->dirlist;
	while (*files)
	{
		if ((new = check_file(dir, files, &dlist, options)))
		{
			*current = new;
			current = &(*current)->next;
			options->nbfiles++;
		}
		files++;
	}
	dir->mlink = ft_numlen(dir->mlink);
	dir->mbyte = ft_numlen(dir->mbyte);
	return (dir);
}

int				main(int argc, char **argv)
{
	t_opt			*options;
	t_dir			*dir;
	int				count;

	options = malloc(sizeof(t_opt));
	ft_memset(options, 0, sizeof(t_opt));
	options->cmp = cmp_alpha;
	options->s = stat;
	count = set_options(++argv, &options);
	ft_qsort(argv + count, argc - count - 1, sizeof(char*), cmp_alphasort);
	dir = read_files(argv + count, options);
	if (argc > 1 && (dir->list || dir->dirlist))
	{
		options->dirarg = 1;
		ft_ls(dir, options);
	}
	else if (options->nberror)
		;
	else
		ft_ls(read_dir(".", options), options);
	return (options->nberror);
}
