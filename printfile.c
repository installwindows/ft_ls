/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printfile.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/07 22:33:55 by varnaud           #+#    #+#             */
/*   Updated: 2017/03/07 22:48:48 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	printfile(const char *name, struct stat *s)
{
	
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
