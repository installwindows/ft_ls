NAME=ft_ls
SRC=tmpmain.c \
	ft_nls.c \
	ft_scandir.c \
	ft_memdup.c \
	ft_dirsize.c \
	ft_qsort.c

.PHONY: test

all: $(NAME)

$(NAME):
	make -C libft/ fclean && make -C libft/
	make -C ft_printf/ fclean && make -C ft_printf/
	gcc $(SRC) -Ilibft -Ift_printf -Llibft -Lft_printf -lft -lftprintf -o $(NAME)

clean:

fclean: clean
	make -C libft/ fclean
	make -C ft_printf/ fclean
	rm -rf $(NAME)

re: fclean all

test:
	gcc $(SRC) -g -Ilibft -Ift_printf -Llibft -Lft_printf -lft -lftprintf -o test
