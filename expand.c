/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:48:42 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/08 20:54:25 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

token_list_t    *expand(token_list_t *tokens)
{
	lol *current_node = tokens->head;
	char *new_value;
	token_t *token;

	while (current_node) 
	{
		token = current_node->token;
		if (token->type == TOKEN_WORD) 
		{
			if (token->quote != '\'')
			{
				new_value = expand_string_variables(token->value);
				if (new_value != token->value)
					token->value = new_value;
			}
		}
		current_node = current_node->next;
	}
    return (tokens);
}

void    init_exp(exp_t *new,char *org)
{
	new->dollar_sign_pos = 0;
	new->current_pos = org;
	new->result_buffer = NULL;
	new->result_len = 0;
	new->result_capacity = 0;
	new->expansions_done = 0;
}

char *expand_string_variables(char *original_value)
{
    if (!original_value)
    return NULL;
	exp_t	*exp;
	size_t  prefix_len;
	int     i;
	char    *var_name_buffer;
	char    *var_char_ptr;
	char	*env_value;
	// bool	curly_braces;
    // exp = init_exp(original_value);
    exp = (exp_t *)gc_malloc(sizeof(exp_t));
	if (!exp)
        return (NULL);
    init_exp(exp, original_value);
    // printf("hani hna\n");
	while ((exp->dollar_sign_pos = ft_strchr(exp->current_pos, '$')) != NULL)
	{
		exp->expansions_done++;
		prefix_len = exp->dollar_sign_pos - exp->current_pos;
		if (prefix_len > 0)
			append_to_buffer(exp, exp->current_pos, prefix_len);
		i = 0;
		var_char_ptr = exp->dollar_sign_pos + 1;
		// curly_braces = false;
		// if (*var_char_ptr == '{') {
		// 	curly_braces = true;
		// 	var_char_ptr++;
		// }
		while (ft_isalnum(*var_char_ptr) || *var_char_ptr == '_')
		{
			// HOME 
			i++;
			var_char_ptr++;
		}
		var_name_buffer = gc_malloc(i + 1);
		ft_memcpy(var_name_buffer, exp->dollar_sign_pos + 1, i);
		var_name_buffer[i] = '\0';
		// if (curly_braces) {
		// 	if (*var_char_ptr == '}')
		// 		var_char_ptr++;
		// 	else
		// 	{
		// 		append_to_buffer(&result_buffer, &result_len, &result_capacity, dollar_sign_pos, (var_char_ptr - dollar_sign_pos));
		// 		current_pos = var_char_ptr;
		// 		continue;
		// 	}
		// }
		if (ft_strlen(var_name_buffer) > 0)
		{
			env_value = ft_getenv(var_name_buffer);
			if (env_value != NULL)
				append_to_buffer(exp, env_value, ft_strlen(env_value));
		}
		// else if (*(exp->dollar_sign_pos + 1) != '\0' && !ft_isspace(*(exp->dollar_sign_pos + 1)))
		// {
		// 	if (*(exp->dollar_sign_pos + 1) == '?' || ft_isspace(*(exp->dollar_sign_pos + 1)) || (!ft_isalpha(*(exp->dollar_sign_pos + 1)) && *(exp->dollar_sign_pos + 1) != '_'))
		// 		append_to_buffer(&result_buffer, &result_len, &result_capacity, "$", 1);
		// 	else
		// 	{
		// 		char temp[3] = {'$', *(exp->dollar_sign_pos + 1), '\0'};
		// 		if (*(exp->dollar_sign_pos + 1) == '\0')
		// 			append_to_buffer(&result_buffer, &result_len, &result_capacity, temp, 1);
		// 		else
		// 			append_to_buffer(&result_buffer, &result_len, &result_capacity, temp, 2);
		// 		if (*(exp->dollar_sign_pos + 1) != '\0')
		// 			var_char_ptr = exp->dollar_sign_pos + 2;
		// 		else
		// 			var_char_ptr = exp->dollar_sign_pos + 1;
		// 	}
		// }
		// else
		// 	append_to_buffer(&result_buffer, &result_len, &result_capacity, "$", 1);
		exp->current_pos = var_char_ptr;
	}
	if (ft_strlen(exp->current_pos) > 0)
		append_to_buffer(exp, exp->current_pos, ft_strlen(exp->current_pos));
	if (exp->expansions_done == 0)
    {
        return (original_value);
    }
	else
	{
        if (!exp->result_buffer)
        exp->result_buffer = ft_strdup("");
		return exp->result_buffer;
	}
}

// void append_to_buffer(char **buffer, size_t *len, size_t *capacity, const char *str_to_add, size_t add_len) {
//     if (*buffer == NULL) {
//         if (add_len + 1 > 256) {
//             *capacity = add_len + 1;
//         } else {
//             *capacity = 256;
//         }
//         *buffer = (char *)malloc(*capacity);
void append_to_buffer(exp_t *exp, char *str_to_add, size_t add_len)
{
	if (exp->result_buffer == NULL)
	{
		exp->result_buffer = (char *)gc_malloc(exp->result_capacity);
		if (!exp->result_buffer) 
		{
			perror("malloc");
			return ;
		}
		exp->result_len = 0;
		exp->result_buffer[0] = '\0';
	}
	if (exp->result_len + add_len + 1 > exp->result_capacity)
	{
		exp->result_capacity = exp->result_len + add_len + 1;
		char *new_buffer = (char *)ft_realloc(exp->result_buffer, exp->result_len, exp->result_capacity);
		if (!new_buffer) {
			perror("realloc");
			return ;
		}
		exp->result_buffer = new_buffer;
	}
	ft_strncat(exp->result_buffer, str_to_add, add_len);
	exp->result_len += add_len;
}
