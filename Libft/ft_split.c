/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 22:38:10 by adenny            #+#    #+#             */
/*   Updated: 2024/11/18 22:48:07 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_array(char **array, int count)
{
	while (count > 0)
		free(array[--count]);
	free(array);
}

static int	count_words(const char *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
	}
	return (count);
}

static char	*allocate_word(const char *s, char c)
{
	int		len;
	char	*word;
	int		i;

	len = 0;
	i = 0;
	while (s[len] && s[len] != c)
		len++;
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[len] = '\0';
	return (word);
}

static int	fill_split(const char *s, char c, char **result)
{
	int	i;

	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			result[i] = allocate_word(s, c);
			if (!result[i])
			{
				free_array(result, i);
				return (0);
			}
			while (*s && *s != c)
				s++;
			i++;
		}
	}
	result[i] = NULL;
	return (1);
}

char	**ft_split(const char *s, char c)
{
	char	**result;
	int		words_count;

	if (!s)
		return (NULL);
	words_count = count_words(s, c);
	result = (char **)malloc(sizeof(char *) * (words_count + 1));
	if (!result)
		return (NULL);
	if (!fill_split(s, c, result))
		return (NULL);
	return (result);
}
