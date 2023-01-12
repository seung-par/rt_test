/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seungjoon <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 20:54:16 by seungjoon         #+#    #+#             */
/*   Updated: 2021/11/30 20:56:55 by seungjoon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	malloc_count(char *str, char c)
{
	char	*flag;
	int		argc;

	argc = 0;
	flag = str;
	while (*str != '\0')
	{
		while (*flag != c && *flag != '\0')
			flag++;
		if ((flag - str) > 0)
			argc++;
		str = flag;
		while (*str == c && *str != '\0')
			str++;
		flag = str;
	}
	return (argc);
}

static void	malloc_free(char **strs, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

static int	str_alloc(char **strs, int n, char *str, char *flag)
{
	char	*set;
	long	len;
	int		i;

	len = flag - str;
	set = (char *)malloc((len + 1) * sizeof(char));
	if (set == NULL)
	{
		malloc_free(strs, n);
		return (0);
	}
	i = 0;
	while (i < len)
	{
		set[i] = str[i];
		i++;
	}
	set[i] = '\0';
	strs[n] = set;
	return (1);
}

static int	set_strs(char **strs, char *str, char c)
{
	char	*flag;
	int		i;

	flag = str;
	i = 0;
	while (*str != '\0')
	{
		while (*flag != c && *flag != '\0')
			flag++;
		if ((flag - str) > 0)
		{
			if (!str_alloc(strs, i, str, flag))
				return (0);
			i++;
		}
		str = flag;
		while (*str == c && *str != '\0')
			str++;
		flag = str;
	}
	strs[i] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	char	*str;

	str = (char *)s;
	strs = (char **)malloc((malloc_count(str, c) + 1) * sizeof(char *));
	if (strs == NULL)
		return (NULL);
	if (!set_strs(strs, str, c))
		return (NULL);
	return (strs);
}
