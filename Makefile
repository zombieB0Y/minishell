CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
SRC = minishell.c welcome.c garbage_collector.c utiles.c start.c lexer.c lexer_utiles.c token.c lexer_manipulation.c tokenizer.c expand.c grammar.c \
builtins/ft_unset.c builtins/ft_builtins.c builtins/ft_pwd.c builtins/ft_env.c builtins/ft_echo.c builtins/ft_exit.c builtins/ft_cd.c builtins/ft_export.c \
execution/ft_execute.c execution/ft_split_n.c execution/ft_strdup_n.c execution/ft_chc_built_redirct.c execution/ft_create_env.c execution/helper_functions.c execution/helper_functions2.c \
ft_valid_long.c remove_quotes.c builtins/ft_export_functions.c builtins/ft_export_functions2.c execution/ft_check_type_of_redir.c read_one_character.c quotes_stuff.c init_functions.c \
heredoc_help_functions.c sig.c errors_functions.c expand_utiles.c expand_utiles2.c stuff.c

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
