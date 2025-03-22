#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

# define GREEN "\033[32m"
# define RED "\033[31m"
# define RESET "\033[0m"

//-------structs--------
typedef struct s_built_in
{
	char				*name;
	int					(*func)(char **cmd, int q, char **env);
}						t_build;

typedef struct s_garbage
{
	void				*ptr;
	struct s_garbage	*next;
}						t_garbage;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

//----functions-----

void					print_welcome(void);
void					cleanup(t_garbage **garbage);
void					add_to_garbage(t_garbage **garbage, void *ptr);

#endif