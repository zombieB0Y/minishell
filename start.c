/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:44:14 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/19 22:47:22 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_command(const char *command)
{
	token_list_t	*tokens;
	anas_list		*list;

	tokens = tokenize(command);
	if (!tokens)
		return (0);
	tokens = capture_heredoc(tokens);
	if (!tokens)
		return (0);
	tokens = expand(tokens);
	if (!tokens)
		return (0);
	tokens = remove_surrounding_quotes(tokens);
	if (!tokens)
		return (0);
	list = grammar_check(tokens);
	if (!list)
		return (0);
	return (ft_execute(list));
}
