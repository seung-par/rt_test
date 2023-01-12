/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungjoon <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 20:52:56 by seungjoon         #+#    #+#             */
/*   Updated: 2021/11/30 20:53:19 by seungjoon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char		temp[10];
	long long	dec;
	int			count;

	count = 0;
	dec = n;
	if (dec < 0)
	{
		dec = -dec;
		write(fd, "-", 1);
	}
	while (1)
	{
		temp[count] = dec % 10 + '0';
		count++;
		dec /= 10;
		if (dec == 0)
			break ;
	}
	while (count > 0)
	{
		write(fd, temp + count - 1, 1);
		count--;
	}
}
