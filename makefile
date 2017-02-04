NAME=ft_ls
SRC=main.c \
	ft_ls.c

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
	gcc $(SRC) -Ilibft -Ift_printf -Llibft -Lft_printf -lft -lftprintf -o test
