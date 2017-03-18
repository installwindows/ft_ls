/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 19:26:40 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/17 22:33:49 by varnaud          ###   ########.fr       */
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

int				print_error(const char *msg)
{
	write(2, "ls: ", 4);
	perror(msg);
	return (1);
}

static char		entry_type(mode_t st_mode)
{
	if (S_ISFIFO(st_mode))
		return ('p');
	if (S_ISCHR(st_mode))
		return ('c');
	if (S_ISDIR(st_mode))
		return ('d');
	if (S_ISBLK(st_mode))
		return ('b');
	if (S_ISREG(st_mode))
		return ('-');
	if (S_ISLNK(st_mode))
		return ('l');
	if (S_ISSOCK(st_mode))
		return ('s');
	return ('-');
}

static void		print_time(time_t t)
{
	time_t	c;
	char	*fulldate;

	c = time(NULL);
	fulldate = ctime(&t);
	ft_printf("%.3s %2d ", fulldate + 4, ft_atoi(fulldate + 8));
	if (c - t > 15780000 || c - t < -15780000)
		ft_printf(" %.4s ", fulldate + ft_strlen(fulldate) - 5);
	else
		ft_printf("%.5s ", fulldate + ft_strlen(fulldate) - 14);
}

int				print_file(t_file *c, t_opt *options, t_dir *dir)
{
	char	buf[255];

	ft_memset(buf, 0, 254);
	if (options->l)
	{
		ft_printf("%c%c%c%c%c%c%c%c%c%c%1s %*d %-*s  %-*s  %*lld ",
		entry_type(c->s.st_mode),
		c->s.st_mode & S_IRUSR ? 'r' : '-',
		c->s.st_mode & S_IWUSR ? 'w' : '-',
		c->s.st_mode & S_ISUID && c->s.st_mode & S_IXUSR ? 's' : c->s.st_mode & S_ISUID ? 'S' : c->s.st_mode & S_IXUSR ? 'x' : '-',
		c->s.st_mode & S_IRGRP ? 'r' : '-',
		c->s.st_mode & S_IWGRP ? 'w' : '-',
		c->s.st_mode & S_ISGID && c->s.st_mode & S_IXGRP ? 's' : c->s.st_mode & S_ISGID ? 'S' : c->s.st_mode & S_IXGRP ? 'x' : '-',
		//c->s.st_mode & S_IXGRP ? 'x' : '-',
		c->s.st_mode & S_IROTH ? 'r' : '-',
		c->s.st_mode & S_IWOTH ? 'w' : '-',
		c->s.st_mode & S_IXOTH ? 'x' : '-',
		c->xattr ? "@" : "", dir->mlink, c->nlink,
		dir->mpw, c->pw->pw_name, dir->mgr, c->gr->gr_name, dir->mbyte, c->size);
		print_time(c->s.st_mtimespec.tv_sec);
		ft_printf("%s", c->e ? c->e->d_name : c->path);
		if (S_ISLNK(c->s.st_mode))
		{
			readlink(c->path, buf, 255);
			ft_printf(" -> %s", buf);
		}
	}
	else
		ft_printf("%s", c->e ? c->e->d_name : c->path);
	ft_printf("\n");
	return (1);
}

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

t_dir			*read_dir(const char *dirname, t_opt *options)
{
	struct dirent	*e;
	t_dir			*dir;
	t_file			**dirlist;
	t_file			**current;
	DIR				*pdir;
	int				d = 0;

	current = setup_dir(&dir);
	dirlist = &dir->dirlist;
	if (!(pdir = opendir(dirname)))
	{
		print_error(ft_strchr(dirname, '/') ? ft_strchr(dirname, '/') + 1 : dirname);
		return (NULL);
	}
	while ((e = readdir(pdir)))
	{
		d++;
		if (options->a == 0 && e->d_name[0] == '.')
			continue ;
		if ((*current = addfile(e, dirname, options)))
		{
			dir->size += (*current)->blocks;
			if ((*current)->nlink > dir->mlink)
				dir->mlink = (*current)->nlink;
			if ((*current)->size > dir->mbyte)
				dir->mbyte = (*current)->size;
			if (ft_strlen((*current)->pw->pw_name) > dir->mpw)
				dir->mpw = ft_strlen((*current)->pw->pw_name);
			if (ft_strlen((*current)->gr->gr_name) > dir->mgr)
				dir->mgr = ft_strlen((*current)->gr->gr_name);
			if (ft_strcmp(e->d_name, ".") &&
				ft_strcmp(e->d_name, "..") && S_ISDIR((*current)->s.st_mode))
			{
				*dirlist = ft_memdup(*current, sizeof(t_file));
				//*current = NULL;
				dirlist = &(*dirlist)->next;
				//continue ;
			}
			current = &(*current)->next;
		}
		else
			options->nberror += print_error(NULL);
	}
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
			ft_mergesort(&dir->list, cmp_alpha);
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
	//options->nberror = 0;
	sort_lists(dir, options);
	bytes = print_dir(dir, options);
	if (options->R || options->dirarg)
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
	return (options->nberror);
}
