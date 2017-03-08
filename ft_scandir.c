/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scandir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/24 06:37:59 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/07 20:21:26 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include "libft.h"
#include "tmp.h"

int		dir_alphasort(const void *a, const void *b)
{
	const struct dirent	*x;
	const struct dirent	*y;

	x = *(struct dirent **)a;
	y = *(struct dirent **)b;
	return (ft_strcmp(x->d_name, y->d_name));
}

int		dirfilter(struct dirent *e)
{
	struct stat	s;

	if (e->d_type == DT_UNKNOWN)
	{
		stat(e->d_name, &s);
		return (S_ISDIR(s.st_mode));
	}
	return (e->d_type == DT_DIR);
}

int		dotfilter(struct dirent *e)
{
	if (e->d_name[0] == '.' && e->d_name[1] == '/')
		return (0);
	return (e->d_name[0] == '.');
}

int		ft_scandir(const char *path, struct dirent ***nlist,
		int (*filter)(struct dirent *), int (*cmp)(const void *, const void *))
{
	DIR				*dirp;
	struct dirent	**list;
	struct dirent	*e;
	size_t			size;
	size_t			i;

	dirp = opendir(path);
	if (dirp == NULL)
		return (-1);
	size = ft_dirsize(path, filter);
	list = malloc(sizeof(struct dirent *) * (size + 1));
	i = 0;
	while ((e = readdir(dirp)))
	{
		if (filter && filter(e))
			continue ;
		list[i++] = ft_memdup(e, e->d_reclen);
	}
	list[i] = NULL;
	closedir(dirp);
	if (size && cmp)
		ft_qsort(list, size, sizeof(struct dirent *), cmp);
	*nlist = list;
	return ((int)size);
}
/*
int		main(int argc, char **argv)
{
	struct dirent	**list;
	char	*path;
	int		size;

	path = ".";
	if (argc > 1)
		path = argv[1];
	size = ft_scandir(path, &list, dirfilter, dir_alphasort);
	if (size < 0)
		return -1;
	printf("size: %d\n", size);
	while (size--)
		printf("%s\n", (*list++)->d_name);
}
*/
