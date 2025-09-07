/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 22:34:07 by adenny            #+#    #+#             */
/*   Updated: 2024/11/18 22:34:12 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			k;
	unsigned char	*ptr;

	k = 0;
	ptr = (unsigned char *)s;
	while (k < n)
	{
		if (ptr[k] == (unsigned char)c)
			return ((void *)&ptr[k]);
		k++;
	}
	return (NULL);
}
