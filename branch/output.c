/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/18 20:50:54 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/18 22:28:19 by varnaud          ###   ########.fr       */
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

void			print_chmod(t_file *c, mode_t m)
{
	ft_printf("%c", entry_type(m));
	ft_printf("%c%c", m & S_IRUSR ? 'r' : '-', m & S_IWUSR ? 'w' : '-');
	if (m & S_ISUID && m & S_IXUSR)
		ft_putchar('s');
	else if (m & S_ISUID)
		ft_putchar('S');
	else
		ft_printf("%c", m & S_IXUSR ? 'x' : '-');
	ft_printf("%c%c", m & S_IRGRP ? 'r' : '-', m & S_IWGRP ? 'w' : '-');
	if (m & S_ISGID && m & S_IXGRP)
		ft_putchar('s');
	else if (m & S_ISGID)
		ft_putchar('S');
	else
		ft_printf("%c", m & S_IXGRP ? 'x' : '-');
	ft_printf("%c%c", m & S_IROTH ? 'r' : '-', m & S_IWOTH ? 'w' : '-');
	if (m & S_ISVTX && !(m & S_IXOTH))
		ft_putchar('T');
	else if (m & S_ISVTX && m & S_IXOTH)
		ft_putchar('t');
	else
		ft_putchar(m & S_IXOTH ? 'x' : '-');
	ft_printf("%1s", c->xattr ? "@" : "");
}

int				print_file(t_file *c, t_opt *options, t_dir *dir)
{
	char	buf[255];

	ft_memset(buf, 0, 254);
	if (options->l)
	{
		print_chmod(c, c->s.st_mode);
		ft_printf(" %*d %-*s  %-*s  ", dir->mlink, c->nlink, dir->mpw,
							c->pw->pw_name, dir->mgr, c->gr->gr_name);
		if (S_ISBLK(c->s.st_mode) || S_ISCHR(c->s.st_mode))
			ft_printf("%d, %d ", major(c->s.st_rdev), minor(c->s.st_rdev));
		else
			ft_printf("%*lld ", dir->mbyte, c->size);
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
