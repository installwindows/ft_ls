#include <stdio.h>
#include <unistd.h>
#include <string.h>
// Function that matches input str with
// given wildcard pattern
int		strmatch(char *str, char *pattern, int n, int m)
{
	int		i;
	int		j;
    // empty pattern can only match with
    // empty string
    if (m == 0)
        return (n == 0);
    // lookup table for storing results of
    // subproblems
    int		lookup[n + 1][m + 1];
    // initailze lookup table to false
    memset(lookup, 0, sizeof(lookup));

    // empty pattern can match with empty string
    lookup[0][0] = 1;
 
    // Only '*' can match with empty string
    j = 1;
	while (j <= m)
	{
        if (pattern[j - 1] == '*')
            lookup[0][j] = lookup[0][j - 1];
		j++;
	}
 
    // fill the table in bottom-up fashion
	i = 1;
    while (i <= n)
    {
		j= 1;
        while (j <= m)
        {
            // Two cases if we see a '*'
            // a) We ignore ‘*’ character and move
            //    to next  character in the pattern,
            //     i.e., ‘*’ indicates an empty sequence.
            // b) '*' character matches with ith
            //     character in input
            if (pattern[j - 1] == '*')
                lookup[i][j] = lookup[i][j - 1] || lookup[i - 1][j];
 
            // Current characters are considered as
            // matching in two cases
            // (a) current character of pattern is '?'
            // (b) characters actually match
            else if (pattern[j - 1] == '?' || str[i - 1] == pattern[j - 1])
                lookup[i][j] = lookup[i - 1][j - 1];
 
            // If characters don't match
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
