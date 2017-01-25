/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 02:17:54 by varnaud           #+#    #+#             */
/*   Updated: 2017/01/25 04:22:29 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h> 
#include <stdio.h> 
#include "libft.h"
#include "ft_ls.h"

void	set_flags(char *arg, int *flags)
{
	while (*arg)
	{
		if (*arg == 'a')
		{
			*flags |= FLAG_SMALL_A;
		}
		else if (*arg == 'l')
		{
			*flags |= FLAG_SMALL_L;
		}
		else if (*arg == 'r')
		{
			*flags |= FLAG_SMALL_R;
		}
		else if (*arg == 'R')
		{
			*flags |= FLAG_R;
		}
		else if (*arg == 't')
		{
			*flags |= FLAG_SMALL_T;
		}
		arg++;
	}
}

void	set_order(char *arg)
{
	while (*arg)
	{
		if (ft_strchr(g_options, *arg))
			*g_setorder++ = *arg;
		arg++;
}

void	display_order(void)
{
	ft_printf("Order: |%s|\n", g_order);
	usage();
}

void	display_flags(int flags)
{
	ft_putstr("Flags: ");
	if (flags & FLAG_SMALL_A)
		ft_putchar('a');
	if (flags & FLAG_SMALL_L)
		ft_putchar('l');
	if (flags & FLAG_SMALL_R)
		ft_putchar('r');
	if (flags & FLAG_R)
		ft_putchar('R');
	if (flags & FLAG_SMALL_T)
		ft_putchar('t');
	ft_putchar('\n');
}

void	usage(void)
{
	ft_printf("usage: ft_ls [%s] [file ...]\n", g_options);
}

int		main(int argc, char **argv)
{
	DIR				*d;
	struct dirent	*dir;
	int				flags;
	char			*directory;

	directory = NULL;
	flags = 0;
	while (*++argv)
	{
		if (**argv == '-')
			set_flags((*argv + 1), &flags);
		else
			//add argv and loop it here
			directory = ft_strdup(*argv);
	}
	display_flags(flags);
	d = opendir(directory == NULL ? "." : directory);
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			ft_putendl(dir->d_name);
		}
		closedir(d);
	}
	return(0);
}
