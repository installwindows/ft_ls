#include <sys/stat.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int		main(int argc, char **argv)
{
	struct stat		s;
	struct passwd	*pw;
	char			buf[255];

	memset(buf, 0, 255);
	memset(&s, 0, sizeof(struct stat));
	if (argc > 1)
	{
		lstat(*++argv, &s);
		if (S_ISREG(s.st_mode))
			printf("Reg\n");
		if (S_ISLNK(s.st_mode))
		{
			//lstat(*argv, &s);
			readlink(*argv, buf, 255);
			printf("Link: %s\n", buf);
		}
		printf("%lld Link? %d\n", s.st_size, s.st_mode & S_IFLNK);
	}
}
