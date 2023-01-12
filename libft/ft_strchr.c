/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungjoon <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 20:57:16 by seungjoon         #+#    #+#             */
/*   Updated: 2021/11/30 20:57:40 by seungjoon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		len;
	char	*str;
	char	character;

	str = (char *)s;
	character = (char)c;
	len = 0;
	while (s[len])
		len++;
	while (0 < len + 1)
	{
		if (*str == character)
			return (str);
		len--;
		str++;
	}
	return (NULL);
}
