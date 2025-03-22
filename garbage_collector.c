#include "minishell.h"

void	add_to_garbage(t_garbage **garbage, void *ptr)
{
	t_garbage	*new;
	t_garbage	*temp;

	new = malloc(sizeof(t_garbage));
	if (!new)
		return ;
	new->ptr = ptr;
	new->next = NULL;
	if (!garbage)
	{
		*garbage = new;
		return ;
	}
	temp = *garbage;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

void	cleanup(t_garbage **garbage)
{
	t_garbage	*current;
	t_garbage	*next;

	current = *garbage;
	while (current)
	{
		next = current->next;
		if (current->ptr)
			free(current->ptr);
		free(current);
		current = next;
	}
	garbage = NULL;
}