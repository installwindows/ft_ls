/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dirsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/05 16:37:39 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/06 16:37:19 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <dirent.h>

/*
int		dotfilter(struct dirent *e)
{
	return (e->d_name[0] != '.');
}
*/

int		ft_dirsize(const char *path, int (*filter)(struct dirent *e))
{
	DIR				*dirp;
	struct dirent	*e;
	int				size;

	dirp = opendir(path);
	if (dirp == NULL)
		return (-1);
	size = 0;
	while ((e = readdir(dirp)))
		if (!filter || !filter(e))
			size++;
	return (size);
}

/*
int		main(int argc, char **argv)
{
	if (argc > 1)
		while (*++argv)
			printf("%s: %d\n", *argv, ft_dirsize(*argv, dotfilter));
	else
		printf("%s: %d\n", ".", ft_dirsize(".", NULL));
}
*/
