/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenba <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:14:45 by abenba            #+#    #+#             */
/*   Updated: 2024/11/01 14:15:58 by abenba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*ptr1;

	if (lst && *lst && del)
	{
		while ((*lst))
		{
			ptr1 = (*lst)->next;
			(*del)((*lst)->content);
			free((*lst));
			(*lst) = ptr1;
		}
		*lst = NULL;
	}
	
}
