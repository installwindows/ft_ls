/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 19:26:40 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/15 23:08:25 by varnaud          ###   ########.fr       */
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
#include "ft_mergesort.c"

int					print_error(const char *msg)
{
	write(2, "ls: ", 4);
	perror(msg);
	return (1);
}

char				*get_path(const char *dname, const char *fname)
{
	char	*path;
	char	*r;

	path = malloc(sizeof(char) * (ft_strlen(dname) + ft_strlen(fname) + 2));
	r = path;
	path[0] = '\0';
	while ((*path = *dname))
	{
		path++;
		dname++;
	}
	if (path[-1] != '/')
	{
		*path = '/';
		*++path = '\0';
	}
	while ((*path = *fname))
	{
		path++;
		fname++;
	}
	return (r);
}

char	entry_type(mode_t st_mode)
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

void	print_time(time_t t)
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

void	*ft_memdup(const void *source, size_t size)
{
	unsigned char	*dup;
	size_t			i;

	dup = malloc(size);
	i = 0;
	while (i < size)
	{
		dup[i] = ((unsigned char*)source)[i];
		i++;
	}
	return (dup);
}

t_file				*addfile(struct dirent *e, const char *dname, t_opt *opt)
{
	t_file			*file;

	file = malloc(sizeof(t_file));
	ft_memset(file, 0, sizeof(t_file));
	file->e = ft_memdup(e, sizeof(struct dirent));
	file->path = get_path(dname, e->d_name);
	if (opt->s(file->path, &file->s) == -1)
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

void				print_file(t_file *c, t_opt *options, t_dir *dir)
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
		print_time(GETTIME(c->s));
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
}

void				print_dir(t_dir *dir, t_opt *options)
{
	t_file	*c;

	c = dir->list;
	if (options->l && dir->list)
		ft_printf("total %d\n", dir->size);
	while (c)
	{
		print_file(c, options, dir);
		c = c->next;
	}
}

t_dlist				*get_dir(t_dir *dir)
{
	t_dlist	*dlist;
	t_dlist	**cur;
	t_file	*file;

	dlist = NULL;
	cur = &dlist;
	file = dir->list;
	while (file)
	{
		if (!ft_strcmp(file->e->d_name, ".") || !ft_strcmp(file->e->d_name, ".."))
		{
			file = file->next;
			continue ;
		}
		if (S_ISDIR(file->s.st_mode))
		{
			(*cur) = malloc(sizeof(t_dlist));
			(*cur)->dirname = ft_strdup(file->path);
			(*cur)->next = NULL;
			cur = &(*cur)->next;
		}
		file = file->next;
	}
	return (dlist);
}

int					ft_ls(const char *dirname, t_opt *options)
{
	struct dirent	*e;
	t_dir			*dir;
	t_dlist			*dirlist;
	t_file			**current;
	DIR				*pdir;
	int				nberror;

	nberror = 0;
	dir = malloc(sizeof(t_dir));
	dir->list = NULL;
	dir->size = 0;
	dir->mlink = 1;
	dir->mbyte = 1;
	dir->mpw = 1;
	dir->mgr = 1;
	current = &dir->list;
	if (!(pdir = opendir(dirname)))
		return (print_error(dirname));
	while ((e = readdir(pdir)))
	{
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
			current = &(*current)->next;
		}
		else
			nberror += print_error(NULL);
	}
	dir->mlink = ft_numlen(dir->mlink);
	dir->mbyte = ft_numlen(dir->mbyte);
	ft_mergesort(&dir->list, options->cmp);
	print_dir(dir, options);
	//TODO free dir
	dirlist = get_dir(dir);
	closedir(pdir);
	if (options->R)
	{
		while (dirlist)
		{
			ft_printf("\n%s:\n", dirlist->dirname);
			ft_ls(dirlist->dirname, options);
			dirlist = dirlist->next;
		}
	}
	return (nberror);
}

void				usage(char c)
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

int					set_options(char **arg, t_opt **options)
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

int					set_files(char **arg, t_dlist **d, t_dlist **f, t_opt *opt)
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

void				print_opt(t_opt *options, t_dlist *d, t_dlist *f)
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

t_file				*get_argfile(const char *name, t_opt *options)
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

void				print_arg_files(t_dlist *files, t_opt *options)
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

int					main(int argc, char **argv)
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
		print_arg_files(filelist, options);
		if ((filelist && dirlist) || (dirlist && dirlist->next))
			ft_printf("%s%s:\n", filelist ? "\n" : "", dirlist->dirname);
		while (dirlist)
		{	
			count += ft_ls(dirlist->dirname, options);
			if (dirlist->next)
				ft_printf("\n%s:\n", dirlist->next->dirname);
			dirlist = dirlist->next;
		}
	}
	else if (count == 0)
		count = ft_ls(".", options);
	return (count);
}
