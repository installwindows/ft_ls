#ifndef FT_LS_H
# define FT_LS_H
# include <stdlib.h>
# include "libft.h"

const char	g_options[] = "alrRtf";

typedef struct		s_opt
{
	unsigned int	f;
	char			*order;
	char			*p_order;
	char			**files;
	char			**p_files;
	int				nb_files;
}					t_opt;

t_opt				*g_opt;

#endif
