CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
SRC = minishell.c welcome.c garbage_collector.c utiles.c start.c lexer.c lexer_utiles.c token.c lexer_manipulation.c ft_execute.c ft_split_exc.c
OBJ = $(SRC:.c=.o)
NAME = minishell
LIBFT = libft/libft.a

all: $(LIBFT) $(NAME) 

$(LIBFT):
	$(MAKE) -C libft

$(NAME): $(OBJ) minishell.h
	$(CC) $(OBJ) $(LIBFT) -lreadline -lhistory -o $(NAME)

clean:
	$(MAKE) -C libft clean
	rm -rf $(OBJ)

fclean: clean
	$(MAKE) -C libft fclean
	rm -rf $(NAME)

re: fclean all
