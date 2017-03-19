/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/14 21:19:06 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/18 22:40:53 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H
# include <dirent.h>
# include <sys/stat.h>
# include <grp.h>
# include <pwd.h>

typedef struct		s_dlist
{
	char			*dirname;
	struct s_dlist	*next;
}					t_dlist;

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
	t_file			*dirlist;
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
	int				cr;
	int				r;
	int				t;
	int				f;
	int				(*cmp)(void *, void *);
	int				(*s)(const char *restrict, struct stat *restrict);
	int				nberror;
	int				dirarg;
	int				nbdir;
	int				nbfiles;
	int				first;
}					t_opt;

void				usage(char c);
int					set_options(char **arg, t_opt **options);

int					ft_ls(t_dir *dir, t_opt *options);
t_dir				*read_dir(const char *dirname, t_opt *options);
int					print_file(t_file *c, t_opt *options, t_dir *dir);
int					print_error(const char *msg);

void				ft_mergesort(t_file **list, int (*cmp)(void *, void *));
int					cmp_alpha(void *a, void *b);
int					cmp_revalpha(void *a, void *b);
int					cmp_time(void *a, void *b);
int					cmp_revtime(void *a, void *b);

t_file				**setup_dir(t_dir **dir);
t_file				*addfile(struct dirent *e, const char *dname, t_opt *opt);
char				*get_path(const char *dname, const char *fname);

void				ft_qsort(void *array, int n, int size,
					int (*cmp)(const void *, const void *));
int					cmp_alphasort(const void *a, const void *b);

#endif
