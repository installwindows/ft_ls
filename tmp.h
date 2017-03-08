#ifndef TMP_H
#define TMP_H

int		ft_scandir(const char *path, struct dirent ***nlist,
		int (*filter)(struct dirent *), int (*cmp)(const void *, const void *));
void	ft_qsort(void *array, int n, int size,
		int (*cmp)(const void *, const void *));
void	*ft_memdup(const void *source, size_t size);
int		ft_dirsize(const char *path, int (*filter)(struct dirent *e));
int		ft_lexisort(const char *s1, const char *s2);
int		dir_alphasort(const void *a, const void *b);
int		dotfilter(struct dirent *e);

#endif
