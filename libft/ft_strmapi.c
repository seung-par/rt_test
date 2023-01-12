/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungjoon <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 21:01:24 by seungjoon         #+#    #+#             */
/*   Updated: 2021/11/30 21:01:52 by seungjoon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	size;
	size_t	i;
	char	*ret;

	size = ft_strlen(s);
	ret = (char *)malloc((size + 1) * sizeof(char));
	if (ret == NULL)
		return (NULL);
	i = 0;
	while (i < size)
	{
		ret[i] = f((unsigned int)i, s[i]);
		i++;
	}
	ret[i] = '\0';
	return (ret);
}
