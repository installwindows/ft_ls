/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 02:17:54 by varnaud           #+#    #+#             */
/*   Updated: 2017/01/18 03:12:36 by varnaud          ###   ########.fr       */
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
