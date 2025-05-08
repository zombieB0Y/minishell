/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:29:26 by codespace         #+#    #+#             */
/*   Updated: 2025/05/08 21:31:32 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void remove_surrounding_quotes(token_list_t *list)
{
    if (list == NULL || list->head == NULL)
        return;
    lol *current_node = list->head;

    while (current_node != NULL)
    {
        token_t *token = current_node->token;

        if (token != NULL && token->value != NULL)
        {
            size_t len = ft_strlen(token->value);

            if (len >= 2)
            {
                char first_char = token->value[0];
                char last_char = token->value[len - 1];

                if ((first_char == '"' && last_char == '"') ||
                    (first_char == '\'' && last_char == '\''))
                {
                    char *new_value = gc_malloc(len - 1);
                    if (new_value == NULL)
                    {
                        perror("malloc");
                        current_node = current_node->next;
                        continue;
                    }
                    ft_strncpy(new_value, token->value + 1, len - 2);
                    new_value[len - 2] = '\0';
                    token->value = new_value;
                }
            }
        }
        current_node = current_node->next;
    }
}
