/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 19:26:40 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/14 00:08:43 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <uuid/uuid.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <sys/xattr.h>
#include "libft.h"
#include "ft_ls.h"
#include "ft_printf.h"

char				*get_path(const char *dname, const char *fname)
{
	char	*path;

	path = malloc(sizeof(char) * (ft_strlen(dname) + ft_strlen(fname) + 1));
	*path = '\0';
	ft_strcat(path, dname);
	ft_strcat(path, "/");
	ft_strcat(path, fname);
	return (path);
}

char	entry_type(mode_t st_mode)
{
	if (st_mode & S_IFIFO)
		return ('p');
	if (st_mode & S_IFCHR)
		return ('c');
	if (st_mode & S_IFDIR)
		return ('d');
	if (st_mode & S_IFBLK)
		return ('b');
	if (st_mode & S_IFREG)
		return ('-');
	if (st_mode & S_IFLNK)
		return ('l');
	if (st_mode & S_IFSOCK)
		return ('s');
	return ('-');
}

void	print_time(time_t t)
{
	time_t	c;
	char	*fulldate;
	char	*month;
	int		day;
	char	*timestamp;

	c = time(NULL);
	fulldate = ctime(&t);
	month = ft_strdup(fulldate + 4);
	month[3] = '\0';
	day = ft_atoi(fulldate + 8);
	if (c - t > 15780000 || c - t < -15780000)
	{
		timestamp = ft_strdup(fulldate + ft_strlen(fulldate) - 5);
		timestamp[4] = '\0';
	}
	else
	{
		timestamp = ft_strdup(fulldate + ft_strlen(fulldate) - 14);
		timestamp[5] = '\0';
	}
	ft_printf("%s %2d %5s ", month, day, timestamp);
	free(month);
	free(timestamp);
}

typedef struct		s_file
{
	struct dirent	*e;
	char			*path;
	struct stat		s;
	struct passwd	*pw;
	struct group	*gr;
	int				size;
	int				nlink;
	int				xattr;
	int				blocks;
	struct s_file	*next;
}					t_file;

typedef struct		s_dir
{
	t_file			*list;
	char			*path;
	int				size;
	int				mlink;
	int				mbyte;
}					t_dir;

typedef struct		s_opt
{
	int				a;
	int				l;
	int				R;
	int				r;
	int				t;
}					t_opt;

typedef struct		s_dlist
{
	char			*dirname;
	struct s_dlist	*next;
}					t_dlist;

t_file				*addfile(struct dirent *e, const char *dname)
{
	t_file			*file;

	file = malloc(sizeof(t_file));
	if (file == NULL)
		return (NULL);
	file->e = e;
	file->path = get_path(dname, e->d_name);
	stat(file->path, &file->s);
	file->pw = getpwuid(file->s.st_uid);
	file->gr = getgrgid(file->s.st_gid);
	file->size = file->s.st_size;
	file->nlink = file->s.st_nlink;
	file->xattr = listxattr(file->path, NULL, 0, 0);
	file->blocks = file->s.st_blocks;
	file->next = NULL;
	return (file);
}

void				print_file(t_file *c, t_opt *options, int mlink, int mbyte)
{
	if (options->l)
	{
		ft_printf("%c%c%c%c%c%c%c%c%c%c%1s %*d %s  %s  %*d ",
		entry_type(c->s.st_mode),
		c->s.st_mode & S_IRUSR ? 'r' : '-',
		c->s.st_mode & S_IWUSR ? 'w' : '-',
		c->s.st_mode & S_IXUSR ? 'x' : '-',
		c->s.st_mode & S_IRGRP ? 'r' : '-',
		c->s.st_mode & S_IWGRP ? 'w' : '-',
		c->s.st_mode & S_IXGRP ? 'x' : '-',
		c->s.st_mode & S_IROTH ? 'r' : '-',
		c->s.st_mode & S_IWOTH ? 'w' : '-',
		c->s.st_mode & S_IXOTH ? 'x' : '-',
		c->xattr ? "@" : "", mlink, c->nlink,
		c->pw->pw_name, c->gr->gr_name, mbyte, c->size);
		print_time(c->s.st_mtimespec.tv_sec);
	}
	ft_printf("%s\n", c->e ? c->e->d_name : c->path);
}

void				print_dir(t_dir *dir, t_opt *options)
{
	t_file	*c;

	c = dir->list;
	if (options->l)
		ft_printf("total %d\n", dir->size);
	while (c)
	{
		print_file(c, options, dir->mlink, dir->mbyte);
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

	dir = malloc(sizeof(t_dir));
	dir->list = NULL;
	dir->size = 0;
	dir->mlink = 1;
	dir->mbyte = 1;
	current = &dir->list;
	if (!(pdir = opendir(dirname)))
		return (1);
	while ((e = readdir(pdir)))
	{
		if (options->a == 0 && e->d_name[0] == '.')
			continue ;
		if ((*current = addfile(e, dirname)))
		{
			dir->size += (*current)->blocks;
			if ((*current)->nlink > dir->mlink)
				dir->mlink = (*current)->nlink;
			if ((*current)->size > dir->mbyte)
				dir->mbyte = (*current)->size;
			current = &(*current)->next;
		}
	}
	dir->mlink = ft_numlen(dir->mlink);
	dir->mbyte = ft_numlen(dir->mbyte);
	//TODO sort dir
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
	return (0);
}

void				usage(char c)
{
	if (c)
		ft_printf("ft_ls: illegal option -- %c\n", c);
	ft_printf("usage: ft_ls [-lRart] [file ...]\n");
	exit(1);
}

int					set_options(char **arg, t_opt **options)
{
	int		count;

	count = 0;
	*options = malloc(sizeof(t_opt));
	ft_memset(*options, 0, sizeof(t_opt));
	while (*arg)
	{
		if (**arg == '-' && (*arg)[1] == '\0')
			return (count);
		if (**arg == '-')
			while (*++(*arg))
			{
				if (**arg == 'a')
					(*options)->a = 1;
				else if (**arg == 'l')
					(*options)->l = 1;
				else if (**arg == 'R')
					(*options)->R = 1;
				else if (**arg == 'r')
					(*options)->r = 1;
				else if (**arg == 't')
					(*options)->t = 1;
				else
					usage(**arg);
			}
		else
			return (count);
		arg++;
		count++;
	}
	return (count);
}

void				set_files(char **arg, t_dlist **d, t_dlist **f)
{
	struct stat	s;

	*d = NULL;
	*f = NULL;
	while (*arg)
	{
		ft_memset(&s, 0, sizeof(struct stat));
		stat(*arg, &s);
		if (S_ISDIR(s.st_mode))
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

t_file				*get_argfile(const char *name)
{
	t_file			*file;

	file = malloc(sizeof(t_file));
	file->e = NULL;
	file->path = (char*)name;
	stat(file->path, &file->s);
	file->pw = getpwuid(file->s.st_uid);
	file->gr = getgrgid(file->s.st_gid);
	file->size = file->s.st_size;
	file->nlink = file->s.st_nlink;
	file->xattr = listxattr(file->path, NULL, 0, 0);
	file->blocks = file->s.st_blocks;
	file->next = NULL;
	return (file);
}

void				print_arg_files(t_dlist *files, t_opt *options)
{
	t_file	*list;
	t_file	**cur;
	int		mlink;
	int		mbyte;

	list = NULL;
	cur = &list;
	mlink = 1;
	mbyte = 1;
	while (files)
	{
		*cur = get_argfile(files->dirname);
		if ((*cur)->nlink > mlink)
			mlink = (*cur)->nlink;
		if ((*cur)->size > mbyte)
			mbyte = (*cur)->size;
		cur = &(*cur)->next;
		files = files->next;
	}
	mlink = ft_numlen(mlink);
	mbyte = ft_numlen(mbyte);
	while (list)
	{
		print_file(list, options, mlink, mbyte);
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
	set_files(argv + count, &dirlist, &filelist);
	//print_opt(options, dirlist, filelist);
	//exit(0);
	if (argc > 1 && (filelist || dirlist))
	{
		print_arg_files(filelist, options);
		if (filelist && dirlist)
			ft_printf("\n%s:\n", dirlist->dirname);
		while (dirlist)
		{	
			ft_ls(dirlist->dirname, options);
			dirlist = dirlist->next;
		}
	}
	else
		ft_ls(".", options);
}
