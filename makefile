NAME=ft_ls
SRC=main.c
LIB=libftprintf.a \
	libft.a

all: $(NAME)

$(NAME):
	gcc $(SRC) $(LIB)
