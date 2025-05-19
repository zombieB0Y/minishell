# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/25 07:46:39 by abenba            #+#    #+#              #
#    Updated: 2025/05/19 22:22:28 by zoentifi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CFLAGS = -Wall -Wextra -Werror -g3
CC = cc
CFILES = ft_atoi.c ft_isascii.c ft_memcmp.c ft_putendl_fd.c ft_strdup.c ft_strlen.c ft_strtrim.c \
ft_bzero.c ft_isdigit.c ft_memcpy.c ft_putnbr_fd.c ft_striteri.c ft_strmapi.c ft_substr.c \
ft_calloc.c ft_isprint.c ft_memmove.c ft_putstr_fd.c ft_strjoin.c ft_strncmp.c ft_tolower.c \
ft_isalnum.c ft_itoa.c ft_memset.c ft_split.c ft_strlcat.c ft_strnstr.c ft_toupper.c \
ft_isalpha.c ft_memchr.c ft_putchar_fd.c ft_strchr.c ft_strlcpy.c ft_strrchr.c \
ft_strcmp.c ft_realloc.c ft_strncpy.c ft_strncat.c substr_dup.c


OBJM = $(CFILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJM)
	ar rcs $(NAME) $(OBJM)

%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJM) $(OBONUS)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
