/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungjoon <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 19:46:57 by seungjoon         #+#    #+#             */
/*   Updated: 2021/12/07 20:56:42 by seungjoon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	whitespace(char c)
{
	char	*space;

	space = "\t\n\v\f\r ";
	while (*space != '\0')
	{
		if (c == *space)
			return (1);
		space++;
	}
	return (0);
}

int	ft_atoi(const char *str)
{
	int			i;
	int			minus;
	long long	output;

	i = 0;
	while (whitespace(str[i]))
		i++;
	output = 0;
	minus = 1;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			minus = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (minus == 1 && output >= 922337203685477580 && str[i] - '0' >= 7)
			return (-1);
		if (minus == -1 && output >= 922337203685477580 && str[i] - '0' >= 8)
			return (0);
		output = output * 10 + str[i] - '0';
		i++;
	}
	return (output * minus);
}
