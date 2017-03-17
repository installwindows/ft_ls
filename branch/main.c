/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/16 19:18:22 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/16 21:55:53 by varnaud          ###   ########.fr       */
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

static int		set_files(char **arg, t_dlist **d, t_dlist **f, t_opt *opt)
{
	struct stat	s;
	int			nberror;

	nberror = 0;
	*d = NULL;
	*f = NULL;
	while (*arg)
	{
		ft_memset(&s, 0, sizeof(struct stat));
		if (opt->s(*arg, &s) == -1 && lstat(*arg, &s) == -1)
				nberror += print_error(*arg);
		else if (S_ISDIR(s.st_mode))
		{
			*d = malloc(sizeof(t_dlist));
			(*d)->dirname = *arg;
			(*d)->next = NULL;
			d = &(*d)->next;
		}
		else
		{
			*f = malloc(sizeof(t_dlist));
			(*f)->dirname = *arg;
			(*f)->next = NULL;
			f = &(*f)->next;
		}
		arg++;
	}
	return (nberror);
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

static t_file	*get_argfile(const char *name, t_opt *options)
{
	t_file			*file;

	file = malloc(sizeof(t_file));
	file->e = NULL;
	file->path = (char*)name;
	if (options->s(file->path, &file->s) == -1)
		if (lstat(file->path, &file->s) == -1)
			return (NULL);
	file->pw = getpwuid(file->s.st_uid);
	file->gr = getgrgid(file->s.st_gid);
	file->size = file->s.st_size;
	file->nlink = file->s.st_nlink;
	file->xattr = m_listxattr(file->path, NULL, 0, 0) > 0;
	file->blocks = file->s.st_blocks;
	file->next = NULL;
	return (file);
}

static void		print_arg_files(t_dlist *files, t_opt *options)
{
	t_file		*list;
	t_file		**cur;
	long long	mlink;
	long long	mbyte;
	t_dir		dir;

	list = NULL;
	cur = &list;
	dir.mlink = 1;
	dir.mbyte = 1;
	dir.mpw = 1;
	dir.mgr = 1;
	while (files)
	{
		*cur = get_argfile(files->dirname, options);
		if ((*cur)->nlink > mlink)
			mlink = (*cur)->nlink;
		if ((*cur)->size > mbyte)
			mbyte = (*cur)->size;
		if (ft_strlen((*cur)->pw->pw_name) > dir.mpw)
			dir.mpw = ft_strlen((*cur)->pw->pw_name);
		if (ft_strlen((*cur)->gr->gr_name) > dir.mgr)
			dir.mgr = ft_strlen((*cur)->gr->gr_name);
		cur = &(*cur)->next;
		files = files->next;
	}
	dir.mlink = ft_numlen(mlink);
	dir.mbyte = ft_numlen(mbyte);
	while (list)
	{
		print_file(list, options, &dir);
		list = list->next;
	}
}

int				main(int argc, char **argv)
{
	t_opt			*options;
	t_dlist			*dirlist;
	t_dlist			*filelist;
	int				count;

	count = set_options(++argv, &options);
	count = set_files(argv + count, &dirlist, &filelist, options);
	//print_opt(options, dirlist, filelist);
	//exit(0);
	if (argc > 1 && (filelist || dirlist))
	{
		/*
		if (!options->f)
		{
			if (options->t)
			{
				ft_mergesort(&filelist, cmp_alpha);
				ft_mergesort(&dirlist, cmp_alpha);
			}
			ft_mergesort(&filelist, options->cmp);
			ft_mergesort(&dirlist, options->cmp);
		}
		*/
		print_arg_files(filelist, options);
		if ((filelist && dirlist) || (dirlist && dirlist->next))
			ft_printf("%s%s:\n", filelist ? "\n" : "", dirlist->dirname);
		while (dirlist)
		{	
			count += ft_ls(read_dir(dirlist->dirname, options), options);
			if (dirlist->next)
				ft_printf("\n%s:\n", dirlist->next->dirname);
			dirlist = dirlist->next;
		}
	}
	else if (count == 0)
		count = ft_ls(read_dir(".", options), options);
	return (count);
}
