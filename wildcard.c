#include <stdio.h>
#include <unistd.h>
#include <string.h>

int		strmatch(char *str, char *pattern, int n, int m)
{
	int		i;
	int		j;
    int		lookup[n + 1][m + 1];

    if (m == 0)
        return (n == 0);
    memset(lookup, 0, sizeof(lookup));
    lookup[0][0] = 1;
    j = 1;
	while (j <= m)
	{
        if (pattern[j - 1] == '*')
            lookup[0][j] = lookup[0][j - 1];
		j++;
	}
	i = 1;
    while (i <= n)
    {
		j= 1;
        while (j <= m)
        {
            if (pattern[j - 1] == '*')
                lookup[i][j] = lookup[i][j - 1] || lookup[i - 1][j];
            else if (pattern[j - 1] == '?' || str[i - 1] == pattern[j - 1])
                lookup[i][j] = lookup[i - 1][j - 1];
            else
				lookup[i][j] = 1;
			j++;
        }
		i++;
    } 
    return lookup[n][m];
}
 
int main()
{
    char str[] = "baaabab";
    char pattern[] = "*****ba*****ab";
    // char pattern[] = "ba*****ab";
    // char pattern[] = "ba*ab";
    // char pattern[] = "a*ab";
    // char pattern[] = "a*****ab";
    // char pattern[] = "*a*****ab";
    // char pattern[] = "ba*ab****";
    // char pattern[] = "****";
    // char pattern[] = "*";
    // char pattern[] = "aa?ab";
    // char pattern[] = "b*b";
    // char pattern[] = "a*a";
    // char pattern[] = "baaabab";
    // char pattern[] = "?baaabab";
    // char pattern[] = "*baaaba*";
 
    if (strmatch(str, pattern, strlen(str), strlen(pattern)))
        printf("Yes!\n");
    else
	   printf("No!\n");	
 
    return 0;
}
