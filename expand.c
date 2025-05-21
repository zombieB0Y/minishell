/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:48:42 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/21 16:11:20 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_until_dollar(t_exp *exp, t_exp_tools *tools)
{
	tools->prefix_len = 0;
	tools->segment_start_ptr = exp->current_pos;
	tools->scan_ptr = exp->current_pos;
	if (*exp->current_pos != '\0')
		while (*tools->scan_ptr != '\0' && *tools->scan_ptr != '$')
			tools->scan_ptr++;
	if (tools->scan_ptr > tools->segment_start_ptr)
		append_to_buffer(exp, tools->segment_start_ptr, tools->scan_ptr
			- tools->segment_start_ptr);
	exp->current_pos = tools->scan_ptr;
}

char	*expand_string_variables_herdoc(char *original_value)
{
	t_exp	*exp;

	exp = init_exp(original_value);
	t_exp_tools *(tools) = init_tools(original_value);
	while ((exp->current_pos - original_value) < (long)tools->org_len)
	{
		read_until_dollar(exp, tools);
		if (*exp->current_pos == '\0')
			break ;
		if (*exp->current_pos == '$')
		{
			if (read_dollar_sign(exp, tools))
				continue ;
			if (!tools->var_name_buffer)
				return (perror("malloc"), NULL);
			actual_expand(exp, tools);
		}
		else
			append_increment(exp);
	}
	return (return_result(exp, original_value));
}

char	*expand_string_variables(char *original_value)
{
	int (res) = 0;
	t_exp *(exp) = init_exp(original_value);
	t_exp_tools *(tools) = init_tools(original_value);
	char (active_quote_char) = 0;
	while ((exp->current_pos - original_value) < (long)tools->org_len)
	{
		handle_exp_quotes(active_quote_char, exp, tools);
		if (*exp->current_pos == '\0')
			break ;
		else if (expand_quotes(exp, &active_quote_char))
			;
		else if (*exp->current_pos == '$')
		{
			res = expand_dollar_sign(exp, tools, active_quote_char);
			if (res == 1)
				continue ;
			else if (res == 2)
				return (NULL);
		}
		else
			append_increment(exp);
	}
	return (return_result(exp, original_value));
}

t_token	*expand_this(t_token *token, t_token_list *tokens, t_lol *current_node)
{
	char	*new_value;

	char *(original_token_value) = token->value;
	new_value = expand_string_variables(original_token_value);
	if (!new_value)
		return (NULL);
	if (new_value != original_token_value)
	{
		if (new_value[0] != '\0')
			token->value = new_value;
		else
		{
			remove_token_node(&tokens->head, current_node);
			current_node = tokens->head;
		}
	}
	return (token);
}

t_token_list	*expand(t_token_list *tokens)
{
	t_token	*token;
	t_lol	*current_node;

	if (!tokens || !tokens->head)
		return (tokens);
	current_node = tokens->head;
	while (current_node)
	{
		token = current_node->token;
		if (token && token->type == TOKEN_WORD && token->value != NULL)
			if (ft_strchr(token->value, '$'))
				if (!expand_this(token, tokens, current_node))
					return (NULL);
		current_node = current_node->next;
	}
	return (tokens);
}
