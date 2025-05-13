#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include "minishell.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

typedef struct s_gnl
{
    int        i;
    int        j;
    int        y;
    int        lenstr;
    char    *str;
    char    *line;
}            t_gnl;

char        *get_next_line(int fd);
int            ft_line_verifier(char *buf);
char        *my_ft_strjoin(char const *s1, char const *s2);
#endif