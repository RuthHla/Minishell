/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 22:55:11 by adenny            #+#    #+#             */
/*   Updated: 2024/11/18 22:56:10 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *a, const char *b, size_t len)
{
	size_t	c;
	size_t	k;

	if (*b == '\0')
		return ((char *)a);
	c = 0;
	while (a[c] && c < len)
	{
		if (a[c] == b[0])
		{
			k = 0;
			while (c + k < len && a[c + k] && b[k] && a[c + k] == b[k])
			{
				k++;
				if (b[k] == '\0')
					return ((char *)&a[c]);
			}
		}
		c++;
	}
	return (NULL);
}
