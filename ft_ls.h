#ifndef FT_LS_H
# define FT_LS_H
# include <stdlib.h>
# include "libft.h"

const char	g_options[] = "alrRtf";

typedef struct	s_opt
{
	int			f;
	char		*order;
	char		*p_order;
	char		**files;
	char		**p_files;
	int			nb_files;
}				t_opt;

t_opt			*g_opt;

void			opt_setup(int argc)
{	
	g_opt = malloc(sizeof(t_opt));
	g_opt->order = ft_strnew(sizeof(g_options) + 1);
	g_opt->p_order = g_opt->order;
	g_opt->files = malloc(sizeof(char*) * argc);
	ft_memset(g_opt->files, 0, sizeof(char*) * argc);
	g_opt->p_files = g_opt->files;
	g_opt->nb_files = 0;
	g_opt->f = 0;
}

/*
char		*g_order;
char		*g_porder;
char		**g_files;
char		**g_pfiles;
*/
#endif
