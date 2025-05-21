/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utiles2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 02:05:24 by zm                #+#    #+#             */
/*   Updated: 2025/05/21 15:54:30 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

void	actual_expand(t_exp *exp, t_exp_tools *tools)
{
	ft_memcpy(tools->var_name_buffer, tools->var_name_start,
		tools->var_name_len);
	tools->var_name_buffer[tools->var_name_len] = '\0';
	tools->env_value = NULL;
	if (tools->var_name_len == 1 && tools->var_name_buffer[0] == '?')
		tools->env_value = ft_itoa(func()->status);
	if (tools->env_value == NULL)
		tools->env_value = ft_getenv(tools->var_name_buffer);
	if (tools->env_value != NULL)
		append_to_buffer(exp, tools->env_value, ft_strlen(tools->env_value));
	exp->expansions_done++;
	exp->current_pos = tools->var_name_end;
}

void	handle_exp_quotes(char active_quote_char, t_exp *exp,
		t_exp_tools *tools)
{
	tools->prefix_len = 0;
	tools->segment_start_ptr = exp->current_pos;
	tools->scan_ptr = exp->current_pos;
	if (active_quote_char == '\'')
		while (*tools->scan_ptr != '\0' && *tools->scan_ptr != '\'')
			tools->scan_ptr++;
	else if (active_quote_char != 0)
		while (*tools->scan_ptr != '\0' && *tools->scan_ptr != active_quote_char
			&& *tools->scan_ptr != '$')
			tools->scan_ptr++;
	else
		while (*tools->scan_ptr != '\0' && *tools->scan_ptr != '$'
			&& *tools->scan_ptr != '\'' && *tools->scan_ptr != '"')
			tools->scan_ptr++;
	if (tools->scan_ptr > tools->segment_start_ptr)
		append_to_buffer(exp, tools->segment_start_ptr, tools->scan_ptr
			- tools->segment_start_ptr);
	exp->current_pos = tools->scan_ptr;
}

char	*return_result(t_exp *exp, char *original_value)
{
	if (exp->result_buffer == NULL && exp->expansions_done == 0)
		return (original_value);
	if (exp->result_buffer == NULL)
	{
		exp->result_buffer = (char *)gc_malloc(1);
		if (exp->result_buffer)
			exp->result_buffer[0] = '\0';
		else
			return (perror("malloc"), NULL);
	}
	return (exp->result_buffer);
}

bool	expand_quotes(t_exp *exp, char *active_quote_char)
{
	if (*exp->current_pos == '\'')
	{
		if (*(active_quote_char) == '\'')
			*(active_quote_char) = 0;
		else if (*(active_quote_char) == 0)
			*(active_quote_char) = '\'';
		append_to_buffer(exp, exp->current_pos, 1);
		exp->current_pos++;
		return (true);
	}
	else if (*exp->current_pos == '"')
	{
		if (*(active_quote_char) == '"')
			*(active_quote_char) = 0;
		else if (*(active_quote_char) == 0)
			*(active_quote_char) = '"';
		append_to_buffer(exp, exp->current_pos, 1);
		exp->current_pos++;
		return (true);
	}
	return (false);
}

bool	read_dollar_sign(t_exp *exp, t_exp_tools *tools)
{
	read_key(exp, tools);
	if (tools->var_name_end > tools->var_name_start)
		tools->var_name_len = tools->var_name_end - tools->var_name_start;
	else if (tools->var_name_end == tools->var_name_start)
	{
		append_to_buffer(exp, exp->dollar_sign_pos, 1);
		exp->current_pos++;
		return (true);
	}
	tools->var_name_buffer = (char *)gc_malloc(tools->var_name_len + 1);
	return (false);
}
