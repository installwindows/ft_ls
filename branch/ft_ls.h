/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/14 21:19:06 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/15 22:10:03 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H
# include <dirent.h>
# include <sys/stat.h>
# include <grp.h>
# include <pwd.h>
# ifdef __APPLE__
#  include <uuid/uuid.h>
#  define m_listxattr(a, b, c, d) listxattr(a, b, c, d)
#  define GETTIME(s) s.st_mtimespec.tv_sec
# elif __linux__
#  define m_listxattr(a, b, c, d) listxattr(a, b, c)
#  define GETTIME(s) s.st_mtim.tv_sec
# endif

typedef struct		s_file
{
	struct dirent	*e;
	char			*path;
	struct stat		s;
	struct passwd	*pw;
	struct group	*gr;
	long long		size;
	int				nlink;
	int				xattr;
	int				blocks;
	struct s_file	*next;
}					t_file;

typedef struct		s_dir
{
	t_file			*list;
	char			*path;
	long long		size;
	long long		mlink;
	long long		mbyte;
	int				mpw;
	int				mgr;
}					t_dir;

typedef struct		s_opt
{
	int				a;
	int				l;
	int				R;
	int				r;
	int				t;
	int				(*cmp)(void *, void *);
	int				(*s)(const char *restrict, struct stat *restrict);
}					t_opt;

typedef struct		s_dlist
{
	char			*dirname;
	struct s_dlist	*next;
}					t_dlist;

#endif
