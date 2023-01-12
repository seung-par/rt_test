/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungjoon <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 21:03:27 by seungjoon         #+#    #+#             */
/*   Updated: 2021/11/30 21:04:00 by seungjoon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		len;
	char	*str;
	char	character;

	str = (char *)s;
	character = (char)c;
	len = 0;
	while (s[len])
		len++;
	str += len;
	while (0 < len + 1)
	{
		if (*str == character)
			return (str);
		len--;
		str--;
	}
	return (NULL);
}
