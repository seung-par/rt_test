/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungjoon <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 21:02:07 by seungjoon         #+#    #+#             */
/*   Updated: 2021/11/30 22:08:34 by seungjoon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char	*str;
	char	*to_find;
	size_t	find_len;

	str = (char *)haystack;
	if (*needle == '\0')
		return (str);
	to_find = (char *)needle;
	find_len = 0;
	while (to_find[find_len])
		find_len++;
	while (0 < len && *str != '\0' && len >= find_len)
	{
		if (*str == *to_find)
		{
			if (ft_strncmp(str, to_find, find_len) == 0)
				return (str);
		}
		str++;
		len--;
	}
	return (NULL);
}
