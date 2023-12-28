/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maburnet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 20:47:18 by maburnet          #+#    #+#             */
/*   Updated: 2023/12/01 20:56:44 by maburnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_wordcount(char *s, char c)
{
	int	i;
	int	words;

	i = 0;
	words = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i] != c && s[i] != '\0')
			words++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (words);
}

int	ft_getwordlen(char *s, char c, int i)
{
	int	len;

	len = 0;
	while (s[i] && s[i] == c)
		i++;
	while (s[i] && s[i] != c)
	{
		i++;
		len++;
	}
	return (len);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		a;
	int		b;
	char	**strs;

	i = 0;
	a = -1;
	if (s == NULL)
		return (NULL);
	strs = ft_calloc(sizeof(char *), (ft_wordcount((char *)s, c) + 1));
	if (!strs)
		return (NULL);
	while (++a < ft_wordcount((char *)s, c))
	{
		b = 0;
		while (s[i] && s[i] == c)
			i++;
		strs[a] = ft_calloc(sizeof(char), (ft_getwordlen((char *)s, c, i) + 1));
		if (!strs[a])
			return (ft_freemalloc(strs, a), NULL);
		while (s[i] && s[i] != c)
			strs[a][b++] = s[i++];
	}
	strs[a] = NULL;
	return (strs);
}