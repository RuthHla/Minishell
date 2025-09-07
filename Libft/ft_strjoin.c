/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 22:49:48 by adenny            #+#    #+#             */
/*   Updated: 2024/11/18 22:51:35 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strcpy_offset(char *dest, const char *src, size_t offset)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		dest[offset + i] = src[i];
		i++;
	}
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined_str;
	size_t	s1_len;
	size_t	s2_len;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	joined_str = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!joined_str)
		return (NULL);
	ft_strcpy_offset(joined_str, s1, 0);
	ft_strcpy_offset(joined_str, s2, s1_len);
	joined_str[s1_len + s2_len] = '\0';
	return (joined_str);
}
