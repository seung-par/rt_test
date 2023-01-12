/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungjoon <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 22:18:52 by seungjoon         #+#    #+#             */
/*   Updated: 2021/11/30 23:05:41 by seungjoon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	srclen;
	size_t	destlen;
	size_t	i;

	srclen = ft_strlen(src);
	destlen = ft_strlen(dst);
	if (dstsize <= destlen)
		return (srclen + dstsize);
	i = 0;
	while ((dstsize > i + destlen + 1) && src[i] != '\0')
	{
		dst[destlen + i] = src[i];
		i++;
	}
	*(dst + destlen + i) = '\0';
	return (srclen + destlen);
}
