/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: varnaud <varnaud@student.42.us.org>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/01 14:37:33 by varnaud           #+#    #+#             */
/*   Updated: 2017/02/02 17:27:10 by varnaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H
# include <dirent.h>
# include <stdlib.h>
# include "libft.h"
# define F_LA 1 << 0
# define F_LL 1 << 1
# define F_LR 1 << 2
# define F_UR 1 << 3
# define F_LT 1 << 4
# define F_LF 1 << 5

const char	g_options[] = "alrRtf";

typedef struct		s_opt
{
	unsigned int	f;
	char			*order;
	char			*p_order;
	char			**files;
	char			**p_files;
	char			**dirs;
	char			**p_dirs;
	int				nb_dirs;
	int				nb_files;
}					t_opt;

typedef struct		s_dir
{
	struct dirent	*file;
	struct s_dir	*next;
}					t_dir;

t_opt				*g_opt;

#endif
