/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 02:17:54 by varnaud           #+#    #+#             */
/*   Updated: 2017/02/03 11:26:21 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include "libft.h"
#include "ft_ls.h"
#include "ft_printf.h"

void	opt_setup(int argc)
{
	g_opt = malloc(sizeof(t_opt));
	g_opt->order = ft_strnew(sizeof(g_options) + 1);
	g_opt->p_order = g_opt->order;
	g_opt->files = malloc(sizeof(char*) * argc);
	ft_memset(g_opt->files, 0, sizeof(char*) * argc);
	g_opt->dirs = malloc(sizeof(char*) * argc);
	ft_memset(g_opt->dirs, 0, sizeof(char*) * argc);
	g_opt->p_files = g_opt->files;
	g_opt->p_dirs = g_opt->dirs;
	g_opt->nb_files = 0;
	g_opt->nb_dirs = 0;
	g_opt->f = 0;
}

void	usage(void)
{
	ft_printf("usage: ft_ls [%s] [file ...]\n", g_options);
	exit(1);
}

void	inserer_dans_l_orifice_prevu_a_cet_effet(char arg, int p)
{
	if (!ft_strchr(g_opt->order, arg))
	{
		*g_opt->p_order++ = arg;
		g_opt->f |= (1 << p);
	}
}

void	set_order(char *arg)
{
	int		p;

	while (*arg)
	{
		if ((p = ft_strichr(g_options, *arg)) != -1)
			inserer_dans_l_orifice_prevu_a_cet_effet(*arg, p);
		else
			usage();
		arg++;
	}
}

void	set_file(char *path)
{
	struct stat	s;

	ft_memset(&s, 0, sizeof(struct stat));
	stat(path, &s);
	if (S_ISREG(s.st_mode))
		g_opt->files[g_opt->nb_files++] = path;
	else
		g_opt->dirs[g_opt->nb_dirs++] = path;
}

void	display_opt(void)
{
	int		i;

	ft_printf("options: |%-*s|\n", sizeof(g_options) - 1, g_opt->order);
	ft_printf("flags..: |%s|\n", g_options);
	ft_printf("set....: |%.*b|\n", sizeof(g_options) -1,
						ft_revbits(g_opt->f) >> (32 - sizeof(g_options) + 1));
	ft_printf("files..:\n");
	i = 0;
	while (i < g_opt->nb_files)
		ft_printf("       :%s\n", g_opt->files[i++]);
	ft_printf("dirs...:\n");
	i = 0;
	while (i < g_opt->nb_dirs)
		ft_printf("       :%s\n", g_opt->dirs[i++]);
}

int		main(int argc, char **argv)
{
	DIR				*d;
	struct dirent	*dir;

	opt_setup(argc);
	while (*++argv)
	{
		if (**argv == '-')
			set_order((*argv + 1));
		else
			set_file(*argv);
	}
	if ((g_opt->nb_files | g_opt->nb_dirs) == 0)
		return (ft_ls("."));
	if ((g_opt->f & F_LF) == 0)
	{
		g_opt->dirs = ft_sort_words(g_opt->dirs, g_opt->nb_dirs);
		g_opt->files = ft_sort_words(g_opt->files, g_opt->nb_files);
	}
	display_opt();
	while (g_opt->p_files)
		ft_ls(g_opt->p_files++);
	while (g_opt->p_dirs)
		ft_ls(g_opt->p_dirs++);
	/*
	d = opendir(files == NULL ? "." : files);
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			ft_putendl(dir->d_name);
		}
		closedir(d);
	}
	*/
	return(0);
}
