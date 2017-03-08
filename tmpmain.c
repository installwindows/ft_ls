#include "ft_ls.h"

int		main(int argc, char **argv)
{
	if (argc > 1)
		while (*++argv)
			ft_ls(*argv);
	else
		ft_ls(".");
}
