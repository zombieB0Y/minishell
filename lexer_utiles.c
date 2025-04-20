#include "minishell.h"

// int	peek_advance(InputBuffer *input)
// {
// 	int	c;

// 	if (input->buffer[input->position])
// 		c = input->buffer[input->position++];
// 	return (c);
// }
int	identifier(int c)
{
	if (is_quoted(c))
		return (1);
	return (0);
}
// int	get_peek(InputBuffer *input)
// {
// 	return (input->buffer[input->position]);
// }