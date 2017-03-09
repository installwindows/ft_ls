/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printfile.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/07 22:33:55 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/08 18:03:54 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <time.h>
#include <uuid/uuid.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include "ft_printf/ft_printf.h"
#include "libft/libft.h"

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
	ft_printf("%s %2d %5s", month, day, timestamp);
	free(month);
	free(timestamp);
}

void	printfile(const char *name, struct stat *s)
{
	struct passwd	*pw;
	struct group	*gr;

	pw = getpwuid(s->st_uid);
	gr = getgrgid(s->st_gid);
	ft_printf("%c%c%c%c%c%c%c%c%c%c  %ld %s  %s  %ld ",
	entry_type(s->st_mode),
	s->st_mode & S_IRUSR ? 'r' : '-', s->st_mode & S_IWUSR ? 'w' : '-', s->st_mode & S_IXUSR ? 'x' : '-',
	s->st_mode & S_IRGRP ? 'r' : '-', s->st_mode & S_IWGRP ? 'w' : '-', s->st_mode & S_IXGRP ? 'x' : '-',
	s->st_mode & S_IROTH ? 'r' : '-', s->st_mode & S_IWOTH ? 'w' : '-', s->st_mode & S_IXOTH ? 'x' : '-',
	s->st_nlink, pw->pw_name,
	gr->gr_name, s->st_size);
	print_time(s->st_mtimespec.tv_sec);
	ft_printf(" %s\n", name);
}

int		main(int argc, char **argv)
{
	struct stat	s;

	if (argc > 1)
	{
		while (*++argv)
		{
			stat(*argv, &s);
			printfile(*argv, &s);
		}
	}
}
