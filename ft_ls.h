#ifndef FT_LS_H
# define FT_LS_H
# define FLAG_SMALL_A 1
# define FLAG_SMALL_L 2
# define FLAG_SMALL_R 4
# define FLAG_R 8
# define FLAG_SMALL_T 16

const char	g_options[] = "alrRt";
char		g_order[sizeof(g_options)];
const char	*g_setorder = g_order;

#endif
