/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungjoon <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 19:58:16 by seungjoon         #+#    #+#             */
/*   Updated: 2021/11/30 23:00:29 by seungjoon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*number_alloc(char *arr, int pos, int minus)
{
	char	*ret;
	int		i;

	ret = (char *)malloc((pos + minus + 1) * sizeof(char));
	if (ret == NULL)
		return (NULL);
	i = 0;
	if (minus)
	{
		ret[i] = '-';
		i++;
	}
	while (pos > 0)
	{
		ret[i] = arr[pos - 1];
		pos--;
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

char	*ft_itoa(int n)
{
	char		arr[10];
	int			pos;
	int			minus;
	long long	dec;

	minus = 0;
	dec = n;
	if (dec < 0)
	{
		dec = -dec;
		minus = 1;
	}
	pos = 0;
	while (1)
	{
		arr[pos] = dec % 10 + '0';
		pos++;
		dec /= 10;
		if (dec == 0)
			break ;
	}
	return (number_alloc(arr, pos, minus));
}
