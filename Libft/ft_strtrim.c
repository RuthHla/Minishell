/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 22:57:15 by adenny            #+#    #+#             */
/*   Updated: 2024/11/18 23:01:00 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_it_in(char c, const char *set)
{
	size_t	k;

	k = 0;
	while (set[k])
	{
		if (set[k] == c)
			return (1);
		k++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	size_t	len_s1;
	char	*new_s1;

	if (!s1 || !set)
		return (NULL);
	len_s1 = ft_strlen(s1);
	start = 0;
	while (s1[start] && is_it_in(s1[start], set))
		start++;
	end = len_s1;
	while (end > start && is_it_in(s1[end - 1], set))
		end--;
	new_s1 = (char *)malloc((end - start + 1) * sizeof(char));
	if (!new_s1)
		return (NULL);
	ft_strlcpy(new_s1, s1 + start, end - start + 1);
	return (new_s1);
}
