#include <sys/stat.h>
#include <stdlib.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct s_node
{
	struct passwd *pw;
	char	buf[1000];
	struct s_node *next;
}				t_node;

void	add(t_node **l, struct passwd *pw)
{
	t_node *n;

	n = malloc(sizeof(t_node));
	n->pw = pw;
	n->next = *l;
	*l = n;
}


int		main(int argc, char **argv)
{
	struct stat		s;
	struct passwd	*pw;
	char			buf[255];
	t_node			*list;

	list = NULL;
	memset(buf, 0, 255);
	memset(&s, 0, sizeof(struct stat));
	if (argc > 1)
	{
		while (*++argv)
		{
			lstat(*argv, &s);
			pw = getpwuid(s.st_uid);
			add(&list, pw);
			printf("%s,", *argv);
		}
	}
	getchar();
}
