/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:48:42 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/18 21:08:37 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_buffer(exp_t *exp, size_t add_len)
{
	exp->result_capacity = add_len + 1;
	exp->result_buffer = (char *)gc_malloc(exp->result_capacity);
	if (!exp->result_buffer)
	{
		perror("malloc");
		return ;
	}
	exp->result_len = 0;
	exp->result_buffer[0] = '\0';
}

void	append_to_buffer(exp_t *exp, char *str_to_add, size_t add_len)
{
	char	*new_buffer;

	if (add_len == 0)
		return ;
	if (exp->result_buffer == NULL)
		setup_buffer(exp, add_len);
	else if (exp->result_len + add_len + 1 > exp->result_capacity)
	{
		exp->result_capacity = exp->result_len + add_len + 1;
		new_buffer = (char *)ft_realloc(exp->result_buffer, exp->result_len,
				exp->result_capacity);
		if (!new_buffer)
		{
			perror("realloc");
			return ;
		}
		exp->result_buffer = new_buffer;
	}
	ft_strncat(exp->result_buffer, str_to_add, add_len);
	exp->result_len += add_len + 1;
}

void	append_increment(exp_t *exp)
{
	append_to_buffer(exp, exp->current_pos, 1);
	exp->current_pos++;
}

exp_t	*init_exp(char *org)
{
	exp_t	*new;

	new = (exp_t *)gc_malloc(sizeof(exp_t));
	if (!new)
		return (perror("malloc"), NULL);
	new->dollar_sign_pos = NULL;
	new->current_pos = org;
	new->result_buffer = NULL;
	new->result_len = 0;
	new->result_capacity = 0;
	new->expansions_done = 0;
	return (new);
}

exp_tools_t	*init_tools(char *or_vl)
{
	exp_tools_t	*tools;

	tools = gc_malloc(sizeof(exp_tools_t));
	if (!tools)
		return (NULL);
	tools->var_name_len = 0;
	tools->org_len = ft_strlen(or_vl);
	tools->segment_start_ptr = NULL;
	tools->var_name_start = NULL;
	tools->var_name_end = NULL;
	tools->var_name_buffer = NULL;
	tools->prefix_len = 0;
	tools->scan_ptr = NULL;
	tools->env_value = NULL;
	return (tools);
}

void	read_key(exp_t *exp, exp_tools_t *tools)
{
	exp->dollar_sign_pos = exp->current_pos;
	tools->var_name_start = exp->current_pos + 1;
	tools->var_name_end = tools->var_name_start;
	if (ft_isalpha(*(tools->var_name_end)) || *(tools->var_name_end) == '_')
	{
		(tools->var_name_end)++;
		while (ft_isalnum(*(tools->var_name_end))
			|| *(tools->var_name_end) == '_')
		{
			(tools->var_name_end)++;
			tools->prefix_len++;
		}
	}
	else if (ft_isdigit(*(tools->var_name_end))
		|| *(tools->var_name_end) == '?')
		(tools->var_name_end)++;
}

void	actual_expand(exp_t *exp, exp_tools_t *tools)
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

void	handle_exp_quotes(char active_quote_char, exp_t *exp,
		exp_tools_t *tools)
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

char	*return_result(exp_t *exp, char *original_value)
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

bool	expand_quotes(exp_t *exp, char *active_quote_char)
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

bool	read_dollar_sign(exp_t *exp, exp_tools_t *tools)
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

int	expand_dollar_sign(exp_t *exp, exp_tools_t *tools, char active_quote_char)
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

void	read_until_dollar(exp_t *exp, exp_tools_t *tools)
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
	exp_t	*exp;

	exp = init_exp(original_value);
	exp_tools_t *(tools) = init_tools(original_value);
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
	int(res) = 0;
	exp_t *(exp) = init_exp(original_value);
	exp_tools_t *(tools) = init_tools(original_value);
	char(active_quote_char) = 0;
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

token_t	*expand_this(token_t *token, token_list_t *tokens, lol *current_node)
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

token_list_t	*expand(token_list_t *tokens)
{
	token_t	*token;
	lol		*current_node;

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
