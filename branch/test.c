#include <sys/stat.h>
#include <sys/xattr.h>
#include <stdlib.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int		main(int argc, char **argv)
{
	struct stat		s;
	char			buf[255] = {0};

	memset(&s, 0, sizeof(struct stat));
	if (argc > 1)
	{
		while (*++argv)
		{
			lstat(*argv, &s);
			printf("%s: %s\n", *argv, listxattr(*argv, NULL, 0, XATTR_NOFOLLOW) > 0 ? "@" : "");
		}
	}
}
