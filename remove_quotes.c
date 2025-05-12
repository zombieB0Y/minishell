/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:29:26 by codespace         #+#    #+#             */
/*   Updated: 2025/05/12 15:06:41 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char    *shift_quotes(char *str, int i)
// {
//     char *new_str;
//     size_t j;

//     new_str = NULL;
//     j = 0;
//     while (str[i] && str[i] != first && str[i] != last)
//         ft_memmove(str + j, str + i, ft_strlen(str) - i);
//     new_str = ft_strdup(str);
//     return (new
// {
//     char *new_str;
//     size_t i;

//     new_str = NULL;
//     i = 0;
//     ft_memmove(str + i, str + i + 1, ft_strlen(str) - i);
//     new_str = ft_strdup(str);
// }

char *shift_quotes(char *str)
{
	// char *new_str;
	size_t i;

	// new_str = NULL;
	char active_quote_char = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			if (active_quote_char == 0)
				active_quote_char = str[i];
			else if (active_quote_char == str[i])
				active_quote_char = 0;
			// i++;
			ft_memmove(str + i, str + i + 1, ft_strlen(str) - i);
		}
		// printf("%s\n", str);
		if (active_quote_char != 0)
		{
			while (str[i] && str[i] != active_quote_char)
				i++;
			// if (str[i] == active_quote_char)
			// {
			// 	active_quote_char = 0;
			// 	ft_memmove(str + i, str + i + 1, ft_strlen(str) - i);
			// 	printf("%s\n", str);
			// 	// i++;
			// }
		}
		else
			while (str[i] && str[i] != '\'' && str[i] != '"')
				i++;
		// else if (str[i] == '\'' && str[i + 1] == '\'' && active_quote_char == 0)
		// 	ft_memmove(str + i, str + i + 2, ft_strlen(str) - i - 1);
		// else if (str[i] == '"' && str[i + 1] == '"' && active_quote_char == 0)
		// 	ft_memmove(str + i, str + i + 2, ft_strlen(str) - i - 1);
		// printf("-----%ld----\n", i);
	}
	// if (i > 0)
	// {
	// 	new_str = ft_strdup(str + i);
	// 	// free(str);
	// 	return (new_str);
	// }
	return (ft_strdup(str));
}

token_list_t *remove_surrounding_quotes(token_list_t *list)
{
	if (!list || !list->head)
		return list;
	lol *current_node = list->head;
	// lol *prev_node = NULL;
	while (current_node->token->type != TOKEN_EOF)	
	{
	// 	if (current_node->token->quote != 69)
	// 	{
			if (ft_strchr(current_node->token->value, '\'') || ft_strchr(current_node->token->value, '"'))
				current_node->token->value = shift_quotes(current_node->token->value);
		// 	if (current_node->token->type != TOKEN_EOF && current_node->token->value[0] == '\0')
		// 	{
		// 		// printf("Empty token value after removing quotes\n");
		// 		// lol *next_node = current_node->next;
		// 		remove_token_node(&list->head, current_node);
		// 		list->size--;
		// 		if (prev_node)
		// 			current_node = prev_node->next;}			
		// 		else
		// 			current_node = list->head;
		// if (!prev_node)
		// 	prev_node = current_node;
		// else
		// 	prev_node = prev_node->next;
		current_node = current_node->next;
	}
	return list;
}


// token_list_t    *remove_surrounding_quotes(token_list_t *list)
// {
//     if (list == NULL || list->head == NULL)
//         return NULL;
//     lol *current_node = list->head;
//     while (current_node != NULL)
//     {
//         token_t *token = current_node->token;

//         if (token != NULL && token->value != NULL)
//         {
//             size_t len = ft_strlen(token->value);

//             if (len >= 2)
//             {
//                 char first_char = token->value[0];
//                 char last_char = token->value[len - 1];

//                 if ((first_char == '"' && last_char == '"') ||
//                     (first_char == '\'' && last_char == '\''))
//                 {
//                     char *new_value = gc_malloc(len - 1);
//                     if (new_value == NULL)
//                     {
//                         perror("malloc");
//                         current_node = current_node->next;
//                         continue;
//                     }
//                     ft_strncpy(new_value, token->value + 1, len - 2);
//                     new_value[len - 2] = '\0';
//                     token->value = new_value;
//                 }
//             }
//         }
//         current_node = current_node->next;
//     }
//     return list;
// }
