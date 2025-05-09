CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
SRC = minishell.c welcome.c garbage_collector.c utiles.c start.c lexer.c lexer_utiles.c token.c lexer_manipulation.c test.c expand.c grammar.c ft_execute.c ft_builtins.c ft_split_n.c ft_strdup_n.c
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
