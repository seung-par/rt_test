/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungjoon <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 20:59:31 by seungjoon         #+#    #+#             */
/*   Updated: 2021/11/30 21:00:13 by seungjoon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	char	*ret;
	size_t	s1_len;
	size_t	s2_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	str = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	ret = str;
	while (*s1 != '\0')
	{
		*str = *s1;
		str++;
		s1++;
	}
	while (*s2 != '\0')
	{
		*str = *s2;
		str++;
		s2++;
	}
	*str = '\0';
	return (ret);
}
