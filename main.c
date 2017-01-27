/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 02:17:54 by varnaud           #+#    #+#             */
/*   Updated: 2017/01/27 09:07:55 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h> 
#include <stdlib.h>
#include "libft.h"
#include "ft_ls.h"
#include "ft_printf.h"

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

void	set_file(char *file)
{
	g_opt->files[g_opt->nb_files++] = file;
}

void	display_opt(void)
{
	int		i;

	ft_printf("options: |%s|\n", g_opt->order);
	ft_printf(".......: |%b|\n", g_opt->f);
	ft_printf("files :\n");
	i = 0;
	while (i < g_opt->nb_files)
		ft_printf("\t%s\n", g_opt->files[i++]);
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
	display_opt();
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
