CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = minishell.c
OBJ = $(SRC:.c=.o)
NAME = minishell
LIBFT = libft/libft.a

all: $(LIBFT) $(NAME) 

$(LIBFT):
	$(MAKE) -C libft

$(NAME): $(OBJ)
	$(CC) -lreadline -lhistory $(OBJ) $(LIBFT) -o $(NAME)

clean:
	$(MAKE) -C libft clean
	rm -rf $(OBJ)

fclean: clean
	$(MAKE) -C libft fclean
	rm -rf $(NAME)

re: fclean all
