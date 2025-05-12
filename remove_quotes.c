/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:29:26 by codespace         #+#    #+#             */
/*   Updated: 2025/05/09 17:56:49 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

token_list_t *remove_surrounding_quotes(token_list_t *list)
{
    if (!list || !list->head)
        return list;

    lol *current_node = list->head;
    while (current_node)
    {
        token_t *token = current_node->token;
        if (token && token->value)
        {
            char *val = token->value;
            size_t len = ft_strlen(val);

            if (len >= 2)
            {
                char first = val[0];
                char last = val[len - 1];

                if ((first == '"' && last == '"') || (first == '\'' && last == '\''))
                {
                    // Check if matching quotes are not escaped and truly wrap the string
                    int i = 1;
                    int balanced = 1;

                    while ((size_t)i < len - 1)
                    {
                        if (val[i] == first)
                        {
                            balanced = 0;
                            break;
                        }
                        i++;
                    }

                    if (balanced)
                    {
                        char *new_val = gc_malloc(len - 1); // len - 2 for content, +1 for \0
                        if (!new_val)
                        {
                            perror("malloc");
                            current_node = current_node->next;
                            continue;
                        }
                        ft_strncpy(new_val, val + 1, len - 2);
                        new_val[len - 2] = '\0';
                        token->value = new_val;
                    }
                }
            }
        }
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
