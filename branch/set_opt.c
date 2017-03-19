/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_opt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/18 22:03:39 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/18 22:41:37 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "ft_ls.h"
#include "libft.h"

static int		check_opt(char **arg, t_opt **options, int *count)
{
	if (**arg == 'a')
		(*options)->a = 1;
	else if (**arg == 'l')
	{
		(*options)->l = 1;
		(*options)->s = lstat;
	}
	else if (**arg == 'R')
		(*options)->cr = 1;
	else if (**arg == 'r')
		(*options)->r = 1;
	else if (**arg == 't' && ((*options)->t = 1))
		(*options)->s = lstat;
	else if (**arg == 'f' && ((*options)->f = 1))
		(*options)->a = 1;
	else if (**arg == '-')
	{
		if (ft_strcmp(*arg, "-") == 0)
			return (++(*count));
		else
			usage(*(*arg));
	}
	else if (ft_strchr("ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1", **arg) == NULL)
		usage(**arg);
	return (0);
}

static int		check_opt_stat(t_opt **options, int count)
{
	if ((*options)->r && (*options)->t)
		(*options)->cmp = cmp_revtime;
	else if ((*options)->r)
		(*options)->cmp = cmp_revalpha;
	else if ((*options)->t)
		(*options)->cmp = cmp_time;
	if ((*options)->l)
		(*options)->s = lstat;
	return (count);
}

int				set_options(char **arg, t_opt **options)
{
	int		count;
	int		b;

	b = 0;
	count = 0;
	while (*arg)
	{
		if (**arg == '-' && (*arg)[1] == '\0')
			break ;
		if (**arg == '-')
		{
			while (*++(*arg))
				if ((b = check_opt(arg, options, &count)))
					break ;
		}
		else
			break ;
		if (b)
			break ;
		arg++;
		count++;
	}
	return (check_opt_stat(options, count));
}
