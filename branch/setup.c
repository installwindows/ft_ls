/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/16 20:10:10 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/16 21:25:15 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <pwd.h>
#include <grp.h>
#include "libft.h"
#include "ft_ls.h"

char			*get_path(const char *dname, const char *fname)
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

t_file			**setup_dir(t_dir **dir)
{
	(*dir) = malloc(sizeof(t_dir));
	(*dir)->list = NULL;
	(*dir)->dirlist = NULL;
	(*dir)->path = NULL;
	(*dir)->size = 0;
	(*dir)->mlink = 1;
	(*dir)->mbyte = 1;
	(*dir)->mpw = 1;
	(*dir)->mgr = 1;
	return (&(*dir)->list);
}

t_file			*addfile(struct dirent *e, const char *dname, t_opt *opt)
{
	t_file			*file;

	file = malloc(sizeof(t_file));
	ft_memset(file, 0, sizeof(t_file));
	if (e)
	{
		file->e = ft_memdup(e, e->d_reclen);
		file->path = get_path(dname, e->d_name);
	}
	else
	{
		file->e = NULL;
		file->path = ft_strdup(dname);
	}
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
