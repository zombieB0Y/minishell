/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:48:42 by zoentifi          #+#    #+#             */
/*   Updated: 2025/05/08 14:28:49 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// hayed l quotes yaaa had bnadem

char    *get_value(char *value, t_env *env)
{
    while (env)
    {
        if (env->key == value)
            return (env->value);
        env = env->next;
    }
    return (NULL);
}

char 

token_list_t    *expand(token_list_t *tokens, t_env *env)
{
    lol *head = tokens->head;
    char    *dolar;
    char     *pos = NULL;
    while (1)
    {
        if (!head)
            break;
		// char	**split = ft_split_n(,)
        // while ((dolar = ft_strchr(head->token->value, '$')) != NULL);
        // {
        //     pos = dolar;
        //     pos++;
        //     // if (pos && ft_isalpha(*pos))
        //     //     pos++;
        //     // else
        //     // {
        //     //     // remove the dolar, if there is no quotes remove a character and save it to the value
            
        //     // }
        //     while (pos && ft_isalnum(*pos))
        //     pos++;
        //     size_t  len = dolar - pos;
        //     char *temp = substr_dup(dolar, len);
        //     char *expand_value = get_value(temp, env);
        //     if (expand_value)
        //     {

		// 	}
        // }
        head = head->next;
    }
    return (tokens);
}
