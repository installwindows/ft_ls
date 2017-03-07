#ifndef TMP_H
#define TMP_H

void	ft_qsort(void *array, int n, int size,
		int (*cmp)(const void *, const void *));
void	*ft_memdup(const void *source, size_t size);
int		ft_dirsize(const char *path, int (*filter)(struct dirent *e));
int		ft_lexisort(const char *s1, const char *s2);

#endif