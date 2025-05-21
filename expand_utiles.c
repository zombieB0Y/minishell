/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utiles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zoentifi <zoentifi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 02:02:26 by zm                #+#    #+#             */
/*   Updated: 2025/05/21 15:54:14 by zoentifi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_buffer(t_exp *exp, size_t add_len)
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

void	append_to_buffer(t_exp *exp, char *str_to_add, size_t add_len)
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

void	append_increment(t_exp *exp)
{
	append_to_buffer(exp, exp->current_pos, 1);
	exp->current_pos++;
}

t_exp_tools	*init_tools(char *or_vl)
{
	t_exp_tools	*tools;

	tools = gc_malloc(sizeof(t_exp_tools));
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

void	read_key(t_exp *exp, t_exp_tools *tools)
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
