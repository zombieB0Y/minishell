#include "minishell.h"

// void	input_init(InputBuffer *input, char *line)
// {
// 	input->buffer = line;
// 	input->column = 0;
// 	input->length = ft_strlen(line);
// 	input->line = 1;
// 	input->position = 0;
// }



int	start(char *line, t_env *g_env)
{
	return (process_command(line, g_env));
}
