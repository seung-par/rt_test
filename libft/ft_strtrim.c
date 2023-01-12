/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungjoon <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 21:04:17 by seungjoon         #+#    #+#             */
/*   Updated: 2021/12/01 14:21:20 by seungjoon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_set(char c, char *set)
{
	if (!set)
		return (0);
	while (*set != '\0')
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	start;
	unsigned int	end;
	char			*charset;
	char			*str;

	str = (char *)s1;
	charset = (char *)set;
	start = 0;
	while (is_set(str[start], charset) && str[start] != '\0')
		start++;
	if (start >= ft_strlen(str))
		return (ft_strdup(""));
	end = ft_strlen(str) - 1;
	while (is_set(str[end], charset))
		end--;
	return (ft_substr(str, start, end - start + 1));
}
