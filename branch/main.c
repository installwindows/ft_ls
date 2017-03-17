/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/16 19:18:22 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/16 23:17:23 by varnaud          ###   ########.fr       */
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

static void		usage(char c)
{
	if (c)
	{
		write(2, "ls: illegal option -- ", 22);
		write(2, &c, 1);
		write(2, "\n", 1);
	}
	write(2, "usage: ft_ls [-lRart] [file ...]\n", 33);
	exit(1);
}

static int		set_options(char **arg, t_opt **options)
{
	int		count;

	count = 0;
	*options = malloc(sizeof(t_opt));
	ft_memset(*options, 0, sizeof(t_opt));
	(*options)->cmp = cmp_alpha;
	(*options)->s = stat;
	while (*arg)
	{
		if (**arg == '-' && (*arg)[1] == '\0')
			break ;
		if (**arg == '-')
			while (*++(*arg))
			{
				if (**arg == 'a')
					(*options)->a = 1;
				else if (**arg == 'l')
				{
					(*options)->l = 1;
					(*options)->s = lstat;
				}
				else if (**arg == 'R')
					(*options)->R = 1;
				else if (**arg == 'r')
					(*options)->r = 1;
				else if (**arg == 't')
				{
					(*options)->t = 1;
					(*options)->s = lstat;
				}
				else if (**arg == 'f')
				{
					(*options)->f = 1;
					(*options)->a = 1;
				}
				else if (**arg == '-')
				{
					if (ft_strcmp(*arg, "-") == 0)
					{
						count++;
						goto AYY;
					}
					else
						usage(*(*arg + 1));
				}
				else if (ft_strchr("ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1", **arg) == NULL)
					usage(**arg);
			}
		else
			break ;
		arg++;
		count++;
	}
AYY:	
	if ((*options)->r && (*options)->t)
		(*options)->cmp = cmp_revtime;
	else if ((*options)->r)
		(*options)->cmp = cmp_revalpha;
	else if ((*options)->t)
		(*options)->cmp = cmp_time;
	return (count);
}

static void		print_opt(t_opt *options, t_dlist *d, t_dlist *f)
{
	ft_printf("-l: %d\n-R: %d\n-a: %d\n-r: %d\n-t: %d\n",
			options->l, options->R, options->a, options->r, options->t);
	ft_printf("Directory:\n");
	while (d)
	{
		ft_printf("\t%s\n", d->dirname);
		d = d->next;
	}
	ft_printf("File:\n");
	while (f)
	{
		ft_printf("\t%s\n", f->dirname);
		f = f->next;
	}
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
		if ((new = addfile(NULL, *files, options)))
		{
			if (S_ISDIR(new->s.st_mode))
			{
				*dlist = new;
				dlist = &(*dlist)->next;
				files++;
				continue ;
			}
			if (new->nlink > dir->mlink)
				dir->mlink = new->nlink;
			if (new->size > dir->mbyte)
				dir->mbyte = new->size;
			if (ft_strlen(new->pw->pw_name) > dir->mpw)
				dir->mpw = ft_strlen(new->pw->pw_name);
			if (ft_strlen(new->gr->gr_name) > dir->mgr)
				dir->mgr = ft_strlen(new->gr->gr_name);
			*current = new;
			current = &(*current)->next;
		}
		else
			options->nberror += print_error(NULL);
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

	count = set_options(++argv, &options);
	dir = read_files(argv + count, options);
	if (argc > 1 && (dir->list || dir->dirlist))
	{
		options->dirarg = 1;
		ft_ls(dir, options);
	}
	else
		ft_ls(read_dir(".", options), options);
	return (options->nberror);
}
