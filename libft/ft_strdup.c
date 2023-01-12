/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungjoon <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 20:57:58 by seungjoon         #+#    #+#             */
/*   Updated: 2021/11/30 20:58:27 by seungjoon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	int		srclen;
	char	*dup;

	srclen = 0;
	while (s1[srclen])
		srclen++;
	dup = (char *)malloc((srclen + 1) * sizeof(char));
	if (dup == NULL)
		return (0);
	i = 0;
	while (i < srclen)
	{
		dup[i] = s1[i];
		i++;
	}
	dup[srclen] = '\0';
	return (dup);
}
