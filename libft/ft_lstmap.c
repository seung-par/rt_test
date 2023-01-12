/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungjoon <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 20:23:19 by seungjoon         #+#    #+#             */
/*   Updated: 2021/11/30 20:23:50 by seungjoon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*start;
	t_list	*curr;
	t_list	*new;

	if (!lst)
		return (NULL);
	curr = ft_lstnew(f(lst->content));
	if (curr == NULL)
		return (NULL);
	lst = lst->next;
	start = curr;
	while (lst)
	{
		new = ft_lstnew(f(lst->content));
		if (new == NULL)
		{
			ft_lstclear(&start, del);
			return (NULL);
		}
		curr->next = new;
		curr = new;
		lst = lst->next;
	}
	return (start);
}
