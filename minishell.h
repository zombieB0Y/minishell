#ifndef MINISHELL_H
#define MINISHELL_H

# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdint.h>
# include <sys/wait.h>
#include <sys/stat.h>
# include <string.h>
# include <errno.h>

#define GREEN   "\033[32m"
#define RESET   "\033[0m"

typedef struct s_built_in
{
    char *name;
    int (*func)(char **cmd, int q, char **env);
} t_build;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

#endif