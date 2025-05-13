/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:48:42 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/13 14:50:37 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void append_to_buffer(exp_t *exp, char *str_to_add, size_t add_len)
{
	if (add_len == 0)
		return ;
	if (exp->result_buffer == NULL)
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
	else if (exp->result_len + add_len + 1 > exp->result_capacity)
	{
		exp->result_capacity = exp->result_len + add_len + 1;
		char *new_buffer = (char *)ft_realloc(exp->result_buffer, exp->result_len, exp->result_capacity);
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

void init_exp(exp_t *new, char *org)
{
	(void)org;
	new->dollar_sign_pos = NULL;
	new->current_pos = NULL;
	new->result_buffer = NULL;
	new->result_len = 0;
	new->result_capacity = 0;
	new->expansions_done = 0;
}

char *expand_string_variables(char *original_value)
{
	if (!original_value)
		return NULL;

	exp_t *exp = (exp_t *)gc_malloc(sizeof(exp_t));
	if (!exp)
	{
		perror("malloc");
		return original_value;
	}
	
	init_exp(exp, original_value);
	exp->current_pos = original_value;
	
	// pid_t pid = getpid();
	size_t	var_name_len;
	size_t	org_len = ft_strlen(original_value);
	char	active_quote_char = 0;
	char	*segment_start_ptr;
	char	*var_name_start;
	char	*var_name_end;
	char	*var_name_buffer;
	size_t	prefix_len;
	char	*scan_ptr;
	char	*env_value;
// -------------------echo "'"$USER"'"-----------------
	while ((exp->current_pos - original_value) < (long)org_len)
	{
		prefix_len = 0;
		segment_start_ptr = exp->current_pos;
		scan_ptr = exp->current_pos;

		if (active_quote_char == '\'')
			while (*scan_ptr != '\0' && *scan_ptr != '\'')
				scan_ptr++;
		else if (active_quote_char != 0)
			while (*scan_ptr != '\0' && *scan_ptr != active_quote_char && *scan_ptr != '$')
				scan_ptr++;
		else
			while (*scan_ptr != '\0' && *scan_ptr != '$' && *scan_ptr != '\'' && *scan_ptr != '"')
				scan_ptr++;

		if (scan_ptr > segment_start_ptr)
			append_to_buffer(exp, segment_start_ptr, scan_ptr - segment_start_ptr);
		// printf("exp->result_buffer = %s\n", exp->result_buffer);
		exp->current_pos = scan_ptr;

		if (*exp->current_pos == '\0')
			break;
		else if (*exp->current_pos == '\'')
		{
			if (active_quote_char == '\'')
				active_quote_char = 0;
			else if (active_quote_char == 0)
				active_quote_char = '\'';
			append_to_buffer(exp, exp->current_pos, 1);
			exp->current_pos++;
		}
		else if (*exp->current_pos == '"')
		{
			if (active_quote_char == '"')
				active_quote_char = 0;
			else if (active_quote_char == 0)
				active_quote_char = '"';
			append_to_buffer(exp, exp->current_pos, 1);
			exp->current_pos++;
		}
		else if (*exp->current_pos == '$')
		{
			if (active_quote_char == '\'')
			{
				append_to_buffer(exp, exp->current_pos, 1);
				exp->current_pos++;
			}
			else
			{
				exp->dollar_sign_pos = exp->current_pos;
				var_name_start = exp->current_pos + 1;
				var_name_end = var_name_start;

				if (ft_isalpha(*var_name_end) || *var_name_end == '_')
				{
					var_name_end++;
					while (ft_isalnum(*var_name_end) || *var_name_end == '_')
					{
						var_name_end++;
						prefix_len++;
					}
				}
				else if (ft_isdigit(*var_name_end) || *var_name_end == '?')
					var_name_end++;
				// else
				// {
				// 	// append_to_buffer(exp, exp->dollar_sign_pos, 1);
				// 	exp->current_pos++;
				// 	continue;
				// }
				if (var_name_end > var_name_start)
					var_name_len = var_name_end - var_name_start;
				else if (var_name_end == var_name_start)
				{
					append_to_buffer(exp, exp->dollar_sign_pos, 1);
					exp->current_pos++;
					continue;
				}
				// if (var_name_len == 0 && (*(var_name_start - 1) == '?'))
				// {
				// 	append_to_buffer(exp, exp->dollar_sign_pos, 1);
				// 	exp->current_pos = var_name_start;
				// }
				// else
				// {
					var_name_buffer = (char *)gc_malloc(var_name_len + 1);
					if (!var_name_buffer)
					{
						perror("malloc");
						if (exp->result_buffer == NULL && exp->expansions_done == 0)
							return original_value;
						return exp->result_buffer;
					}
					ft_memcpy(var_name_buffer, var_name_start, var_name_len);
					var_name_buffer[var_name_len] = '\0';

					env_value = NULL;
					if (var_name_len == 1 && var_name_buffer[0] == '?')
						env_value = ft_itoa(func()->status);
					// else if (var_name_len == 1 && var_name_buffer[0] == '$')
					//     env_value = ft_itoa(pid);
					if (env_value == NULL)
						env_value = ft_getenv(var_name_buffer);
					if (env_value != NULL)
						append_to_buffer(exp, env_value, ft_strlen(env_value));
					exp->expansions_done++;
					exp->current_pos = var_name_end;
				// }
			}
		}
		else
		{
			append_to_buffer(exp, exp->current_pos, 1);
			exp->current_pos++;
		}
	}
	if (exp->result_buffer == NULL && exp->expansions_done == 0)
		return original_value;
	if (exp->result_buffer == NULL)
	{
		exp->result_buffer = (char *)gc_malloc(1);
		if (exp->result_buffer)
			exp->result_buffer[0] = '\0';
		else
		{
			perror("malloc");
			return original_value;
		}
	}
	return exp->result_buffer;
}

token_list_t *expand(token_list_t *tokens)
{
	if (!tokens || !tokens->head)
		return tokens;

	lol *current_node = tokens->head;
	// lol *prev_node = NULL;
	char *new_value;
	token_t *token;

	while (current_node)
	{
		token = current_node->token;
		if (token && token->type == TOKEN_WORD && token->value != NULL)
		{
			if (ft_strchr(token->value, '$'))
			{
				char *original_token_value = token->value;
				new_value = expand_string_variables(original_token_value);
				if (new_value != original_token_value)
				{
					if (new_value[0] != '\0')
					{
						token->value = new_value;
						// token->quote = 69;
					}
					else
					{
						// printf("Empty token value after expansion\n");
						remove_token_node(&tokens->head, current_node);
						current_node = tokens->head;
					}
				}
			}
		}
		// prev_node = current_node;
		current_node = current_node->next;
	}
	return tokens;
}

// void append_to_buffer(exp_t *exp, char *str_to_add, size_t add_len)
// {
// 	if (add_len == 0)
// 		return ;
// 	if (exp->result_buffer == NULL)
// 	{
// 		exp->result_capacity = add_len + 1;
// 		exp->result_buffer = (char *)gc_malloc(exp->result_capacity);
// 		if (!exp->result_buffer) 
// 		{
// 			perror("malloc");
// 			return ;
// 		}
// 		exp->result_len = 0;
// 		exp->result_buffer[0] = '\0';
// 	}
// 	else
// 	{
// 		exp->result_capacity = exp->result_len + add_len + 1;
// 		char *new_buffer = (char *)ft_realloc(exp->result_buffer, exp->result_len, exp->result_capacity);
// 		if (!new_buffer)
// 		{
// 			perror("realloc");
// 			return ;
// 		}
// 		exp->result_buffer = new_buffer;
// 	}
// 	ft_strncat(exp->result_buffer, str_to_add, add_len);
// 	exp->result_len += add_len;
// }

// token_list_t    *expand(token_list_t *tokens)
// {
// 	lol *current_node = tokens->head;
// 	char *new_value;
// 	token_t *token;

// 	while (current_node) 
// 	{
// 		token = current_node->token;
// 		if (token && token->type == TOKEN_WORD && token->value != NULL)
// 		{
//             if (ft_strchr(token->value, '$'))
// 			{
//                 new_value = expand_string_variables(token->value);
//                 if (new_value != token->value)
//                     token->value = new_value;
//             }
//         }
// 		current_node = current_node->next;
// 	}
//     return (tokens);
// }

// void    init_exp(exp_t *new,char *org)
// {
// 	new->dollar_sign_pos = 0;
// 	new->current_pos = org;
// 	new->result_buffer = NULL;
// 	new->result_len = 0;
// 	new->result_capacity = 0;
// 	new->expansions_done = 0;
// }

// char *expand_string_variables(char *original_value)
// {
//     if (!original_value)
//     {
//         return NULL;
//     }
// 	exp_t	*exp;
// 	size_t  prefix_len;
// 	// int     i;
// 	char    *var_name_buffer;
// 	char	active_quote_char;
// 	// char    *var_char_ptr;
// 	char	*env_value;
// 	char	*end_of_string;
// 	char	*segment_start_ptr;
// 	exp = (exp_t *)gc_malloc(sizeof(exp_t));
// 	if (!exp)
//         return (NULL);
//     init_exp(exp, original_value);
//     // printf("hani hna\n");
// 	active_quote_char = 0;
// 	end_of_string = original_value + ft_strlen(original_value);
// 	segment_start_ptr = original_value;
// 	while (exp->current_pos < end_of_string)
// 	{
// 		while (*exp->current_pos != '\0')
//         {
//             if (*exp->current_pos == '\'' && active_quote_char == 0)
//                 active_quote_char = *exp->current_pos;
//             else if (active_quote_char == '\'' && *exp->current_pos == active_quote_char)
//                 active_quote_char = 0;
//             else if (active_quote_char != '\'' && (*exp->current_pos == '$' || *exp->current_pos == '"'))
//                 break;
// 			exp->current_pos++;
//         }
//         // printf("exp->current_pos = %s\n", exp->current_pos);
// 		prefix_len = exp->current_pos - segment_start_ptr;
// 		if (prefix_len > 0)
// 			append_to_buffer(exp, segment_start_ptr, prefix_len);
// 		// printf("exp->result_buffer = %s\n", exp->result_buffer);
// 		segment_start_ptr = exp->current_pos;
// 		if (*exp->current_pos == '$')
// 		{
// 			// if (active_quote_char == 0)
// 			// 	active_quote_char = *exp->current_pos;
// 			// else if (active_quote_char == *exp->current_pos)
// 			// 	active_quote_char = 0;
//             exp->dollar_sign_pos = exp->current_pos;
//             char *var_name_start = exp->current_pos + 1;
//             char *var_name_end = var_name_start;

//             if (ft_isalpha(*var_name_end) || *var_name_end == '_')
//             {
//                 var_name_end++;
//                 while (ft_isalnum(*var_name_end) || *var_name_end == '_')
//                     var_name_end++;
//             }
//             // else
//             // {
//             //     append_to_buffer(exp, exp->dollar_sign_pos, 1);
//             //     exp->current_pos++;
//             //     continue;
//             // }

//             size_t var_name_len = var_name_end - var_name_start;
//             if (var_name_len == 0 &&
//                 !(ft_isdigit(*(var_name_start - 1)) ||
//                     *(var_name_start - 1) == '?' ||
//                     *(var_name_start - 1) == '$' ||
//                     *(var_name_start - 1) == '#'))
//             {
//                 append_to_buffer(exp, exp->dollar_sign_pos, 1);
//                 exp->current_pos = var_name_start;
//             }
//             else
//             {
//                 char *var_name_buffer = (char *)gc_malloc(var_name_len + 1);
//                 if (!var_name_buffer)
//                 {
//                     perror("gc_malloc for var_name_buffer");
//                     if (exp->result_buffer == NULL && exp->expansions_done == 0)
//                         return original_value;
//                     return exp->result_buffer;
//                 }
//                 ft_memcpy(var_name_buffer, var_name_start, var_name_len);
//                 var_name_buffer[var_name_len] = '\0';

//                 char *env_value = NULL;
//                 // if (var_name_len == 1 && var_name_buffer[0] == '?')
//                 //     env_value = "0";
//                 // else if (var_name_len == 1 && var_name_buffer[0] == '$')
//                 //     env_value = "12345";
//                 if (env_value == NULL)
//                     env_value = ft_getenv(var_name_buffer);
//                 if (env_value != NULL)
//                     append_to_buffer(exp, env_value, ft_strlen(env_value));
//                 exp->expansions_done++;
//                 exp->current_pos = var_name_end;
//                 segment_start_ptr = exp->current_pos;
//                 printf("exp->current_pos = %s\n", exp->current_pos);
//             }
// 			// exp->current_pos++;
// 		}
// 		else if (*exp->current_pos == '\"')
// 		{
// 			exp->expansions_done++;
// 			exp->dollar_sign_pos = exp->current_pos;
// 			exp->current_pos++;
// 			segment_start_ptr = exp->current_pos;
// 			while (*exp->current_pos != '\0' && !is_whitespace(*exp->current_pos))
// 			{
// 				if (*exp->current_pos == '$' || *exp->current_pos == '\"' || *exp->current_pos == '\'')
// 					break ;
// 				exp->current_pos++;
// 			}
// 			if (active_quote_char != '\'')
// 			{
// 				prefix_len = exp->current_pos - segment_start_ptr;
// 				var_name_buffer = gc_malloc(prefix_len + 1);
// 				ft_memcpy(var_name_buffer, segment_start_ptr, prefix_len);
// 				var_name_buffer[prefix_len] = '\0';
// 				env_value = ft_getenv(var_name_buffer);
// 				if (env_value != NULL)
// 					append_to_buffer(exp, env_value, ft_strlen(env_value));
// 			}
// 			else
// 			{
// 				prefix_len = exp->current_pos - segment_start_ptr;
// 				if (prefix_len > 0)
// 					append_to_buffer(exp, segment_start_ptr - 1, prefix_len + 1);
// 			}
// 			segment_start_ptr = exp->current_pos;
// 		}
// 		// if (*exp->current_pos && )
// 		// 	exp->current_pos++;
// 	}
// 	if (exp->result_buffer == NULL && exp->expansions_done == 0)
// 		return (original_value);
// 	return (exp->result_buffer);
// }
	// 	if (*exp->current_pos == '\'' || *exp->current_pos == '\"')
	// 	{
	// 		if (active_quote_char == 0)
	// 			active_quote_char = *exp->current_pos;
	// 		else if (active_quote_char == *exp->current_pos)
	// 			active_quote_char = 0;
	// 		exp->current_pos++;
	// 		continue;
	// 	}
	// 	else if (*exp->current_pos == '$' && active_quote_char == 0)
	// 	{
	// 		printf("expansion\n");
	// 		exp->expansions_done++;
	// 		exp->dollar_sign_pos = exp->current_pos;
	// 		exp->current_pos++;
	// 		segment_start_ptr = exp->current_pos;
	// 		while (*exp->current_pos != '\0' && !is_whitespace(*exp->current_pos))
	// 		{
	// 			if (*exp->current_pos == '$' || *exp->current_pos == '\"' || *exp->current_pos == '\'')
	// 				break ;
	// 			exp->current_pos++;
	// 		}
	// 		prefix_len = exp->current_pos - segment_start_ptr;
	// 		printf("prefix_len = %zu\n", prefix_len);
	// 		// if (prefix_len > 0)
	// 		// 	append_to_buffer(exp, segment_start_ptr, prefix_len);
	// 		// exp->current_pos++;
	// 		// segment_start_ptr = exp->current_pos;
	// 		// prefix_len = exp->current_pos - segment_start_ptr;
	// 		var_name_buffer = gc_malloc(prefix_len + 1);
	// 		ft_memcpy(var_name_buffer, segment_start_ptr, prefix_len);
	// 		var_name_buffer[prefix_len] = '\0';
	// 		printf("var_name_buffer = %s\n", var_name_buffer);
	// 		env_value = ft_getenv(var_name_buffer);
	// 		printf("env_value = %s\n", env_value);
	// 		if (env_value != NULL)
	// 			append_to_buffer(exp, env_value, ft_strlen(env_value));
	// 	}
	// 	else if (*exp->current_pos == '$' && active_quote_char != 0)
	// 	{
	// 		exp->expansions_done++;
	// 		if (active_quote_char != '\'')
	// 		{
	// 			prefix_len = exp->current_pos - segment_start_ptr;
	// 			append_to_buffer(exp, segment_start_ptr, prefix_len);
	// 			exp->current_pos++;
	// 			segment_start_ptr = exp->current_pos;
	// 			while (*exp->current_pos != active_quote_char && *exp->current_pos != '\0')
	// 			{
	// 				if (*exp->current_pos == '$')
	// 				{
	// 					exp->current_pos++;
	// 					break ;
	// 				}
	// 				exp->current_pos++;
	// 			}
	// 			prefix_len = exp->current_pos - segment_start_ptr;
	// 			var_name_buffer = gc_malloc(prefix_len + 1);
	// 			ft_memcpy(var_name_buffer, segment_start_ptr, prefix_len);
	// 			var_name_buffer[prefix_len] = '\0';
	// 			env_value = ft_getenv(var_name_buffer);
	// 			if (env_value != NULL)
	// 			{
	// 				append_to_buffer(exp, env_value, ft_strlen(env_value));
	// 			}
	// 		}
	// 		else if (active_quote_char == '\'')
	// 		{
	// 			exp->current_pos++;
	// 			segment_start_ptr = exp->current_pos;
	// 			while (*exp->current_pos != '\'' && *exp->current_pos != '\0')
	// 			{
	// 				if (*exp->current_pos == '$')
	// 					break ;
	// 				exp->current_pos++;
	// 			}
	// 			prefix_len = exp->current_pos - segment_start_ptr;
	// 			printf("prefix_len ' = %zu\n", prefix_len);
	// 			if (prefix_len > 0)
	// 				append_to_buffer(exp, segment_start_ptr, prefix_len);
	// 			printf("exp->result_buffer = %s\n", exp->result_buffer);
	// 			exp->current_pos++;
	// 			segment_start_ptr = exp->current_pos;
	// 		}
	// 	}
	// 	if (*exp->current_pos)
	// 		exp->current_pos++;
	// }
	// if (exp->result_buffer == NULL && exp->expansions_done == 0)
	// 	return (original_value);
	// return (exp->result_buffer);
// }
	// {
	// 	exp->expansions_done++;
	// 	prefix_len = exp->dollar_sign_pos - exp->current_pos;
	// 	if (prefix_len > 0)
	// 		append_to_buffer(exp, exp->current_pos, prefix_len);
	// 	i = 0;
	// 	var_char_ptr = exp->dollar_sign_pos + 1;
	// 	// curly_braces = false;
	// 	// if (*var_char_ptr == '{') {
	// 	// 	curly_braces = true;
	// 	// 	var_char_ptr++;
	// 	// }
	// 	while (ft_isalnum(*var_char_ptr) || *var_char_ptr == '_')
	// 	{
	// 		// HOME 
	// 		i++;
	// 		var_char_ptr++;
	// 	}
	// 	var_name_buffer = gc_malloc(i + 1);
	// 	ft_memcpy(var_name_buffer, exp->dollar_sign_pos + 1, i);
	// 	var_name_buffer[i] = '\0';
	// 	// if (curly_braces) {
	// 	// 	if (*var_char_ptr == '}')
	// 	// 		var_char_ptr++;
	// 	// 	else
	// 	// 	{
	// 	// 		append_to_buffer(&result_buffer, &result_len, &result_capacity, dollar_sign_pos, (var_char_ptr - dollar_sign_pos));
	// 	// 		current_pos = var_char_ptr;
	// 	// 		continue;
	// 	// 	}
	// 	// }
	// 	if (ft_strlen(var_name_buffer) > 0)
	// 	{
	// 		env_value = ft_getenv(var_name_buffer);
	// 		if (env_value != NULL)
	// 			append_to_buffer(exp, env_value, ft_strlen(env_value));
	// 	}
	// 	// else if (*(exp->dollar_sign_pos + 1) != '\0' && !ft_isspace(*(exp->dollar_sign_pos + 1)))
	// 	// {
	// 	// 	if (*(exp->dollar_sign_pos + 1) == '?' || ft_isspace(*(exp->dollar_sign_pos + 1)) || (!ft_isalpha(*(exp->dollar_sign_pos + 1)) && *(exp->dollar_sign_pos + 1) != '_'))
	// 	// 		append_to_buffer(&result_buffer, &result_len, &result_capacity, "$", 1);
	// 	// 	else
	// 	// 	{
	// 	// 		char temp[3] = {'$', *(exp->dollar_sign_pos + 1), '\0'};
	// 	// 		if (*(exp->dollar_sign_pos + 1) == '\0')
	// 	// 			append_to_buffer(&result_buffer, &result_len, &result_capacity, temp, 1);
	// 	// 		else
	// 	// 			append_to_buffer(&result_buffer, &result_len, &result_capacity, temp, 2);
	// 	// 		if (*(exp->dollar_sign_pos + 1) != '\0')
	// 	// 			var_char_ptr = exp->dollar_sign_pos + 2;
	// 	// 		else
	// 	// 			var_char_ptr = exp->dollar_sign_pos + 1;
	// 	// 	}
	// 	// }
	// 	// else
	// 	// 	append_to_buffer(&result_buffer, &result_len, &result_capacity, "$", 1);
	// 	exp->current_pos = var_char_ptr;
	// }
	// if (ft_strlen(exp->current_pos) > 0)
	// 	append_to_buffer(exp, exp->current_pos, ft_strlen(exp->current_pos));
	// if (exp->expansions_done == 0)
	// {
	//     return (original_value);
	// }
	// else
	// {
	//     if (!exp->result_buffer)
	//     exp->result_buffer = ft_strdup("");
	// 	return exp->result_buffer;
	// }
// }

// void append_to_buffer(char **buffer, size_t *len, size_t *capacity, const char *str_to_add, size_t add_len) {
//     if (*buffer == NULL) {
//         if (add_len + 1 > 256) {
//             *capacity = add_len + 1;
//         } else {
//             *capacity = 256;
//         }
//         *buffer = (char *)malloc(*capacity);
