/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nls.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/07 13:19:31 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/09 00:09:42 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include "tmp.h"
#include "ft_ls.h"
#include "libft.h"
#include "ft_printf.h"

char	*getfullpath(const char *path, const char *name)
{
	char	*fullpath;


	fullpath = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(name) + 2));
	fullpath[0] = '\0';
	ft_strcat(fullpath, path);
	ft_strcat(fullpath, "/");
	ft_strcat(fullpath, name);
	return (fullpath);
}

void	getstat(char *path, char *name, struct stat *s, t_dir ***current)
{
	char	*pathname;
	t_dir	*dir;

	pathname = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(name) + 2));
	pathname[0] = '\0';
	ft_strcat(pathname, path);
	ft_strcat(pathname, "/");
	ft_strcat(pathname, name);
	stat(pathname, s);
	if (S_ISDIR(s->st_mode) && name[0] != '.')
	{
		dir = malloc(sizeof(t_dir));
		dir->path = pathname;
		dir->next = NULL;
		**current = dir;
		*current = &(dir->next);
	}
}

t_dirinfo	getdirinfo(const char *path, int (*filter)(struct dirent *e))
{
	t_dirinfo		dirinfo;
	DIR				*dirp;
	struct dirent	*e;
	struct stat		s;
	char			*fp;

	dirinfo.size = 0;
	dirinfo.linkwidth = 1;
	dirinfo.bytewidth = 1;
	dirp = opendir(path);
	while ((e = readdir(dirp)))
	{
		if (filter && filter(e))
			continue ;
		stat((fp = getfullpath(path, e->d_name)), &s);
		free(fp);
		dirinfo.size += s.st_blocks;
		if (s.st_size > dirinfo.bytewidth)
			dirinfo.bytewidth = s.st_size;
		if (s.st_nlink > dirinfo.linkwidth)
			dirinfo.linkwidth = s.st_nlink;
	}
	dirinfo.bytewidth = ft_numlen(dirinfo.bytewidth);
	dirinfo.linkwidth = ft_numlen(dirinfo.linkwidth);
	return (dirinfo);
}

int		ft_listdir(char *path)
{
	struct dirent	**list;
	int				size;
	t_dir			*dirlist;
	t_dir			**current;
	t_dirinfo		dirinfo;
	struct stat		s;
	char			*fp;

	dirlist = NULL;
	current = &dirlist;
	size = ft_scandir(path, &list, dotfilter, dir_alphasort);
	dirinfo = getdirinfo(path, dotfilter);
	ft_printf("total %ld\n", dirinfo.size);
	if (size < 0)
		return (-1);
	while (size--)
	{
		getstat(path, (*list)->d_name, &s, &current);
		printfile((*list)->d_name, (fp = getfullpath(path, (*list)->d_name)), &s, dirinfo);
		//ft_printf("%s\n", (*list)->d_name);
		free(*list);
		list++;
	}
	while (dirlist)
	{
		ft_printf("\n%s:\n", dirlist->path);
		ft_listdir(dirlist->path);
		dirlist = dirlist->next;
	}
	return (0);
}

int		ft_ls(char *dirname)
{
	/*
	t_dirinfo	dirinfo;

	dirinfo = getdirinfo(dirname, dotfilter);
	printf("total %ld %d %d\n", dirinfo.size, dirinfo.linkwidth, dirinfo.bytewidth);
	*/
	ft_listdir(dirname);
	return (0);
}
