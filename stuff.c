/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stuff.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 02:06:35 by zm                #+#    #+#             */
/*   Updated: 2025/05/21 15:55:25 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_dollar_sign(t_exp *exp, t_exp_tools *tools, char active_quote_char)
{
	if (active_quote_char == '\'')
	{
		append_to_buffer(exp, exp->current_pos, 1);
		exp->current_pos++;
	}
	else
	{
		if (read_dollar_sign(exp, tools))
			return (1);
		if (!tools->var_name_buffer)
			return (perror("malloc"), 2);
		actual_expand(exp, tools);
	}
	return (0);
}
