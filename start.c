#include "minishell.h"

void	input_init(InputBuffer *input, char *line)
{
	input->buffer = line;
	input->column = 0;
	input->length = ft_strlen(line);
	input->line = 1;
	input->position = 0;
}

void	start(char *line)
{
	process_command(line);
}
