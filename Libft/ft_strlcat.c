/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 22:53:12 by adenny            #+#    #+#             */
/*   Updated: 2024/11/18 22:53:18 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	d;
	size_t	e;
	size_t	c;

	d = ft_strlen(dst);
	e = ft_strlen(src);
	if (dstsize <= d)
		return (dstsize + e);
	c = 0;
	while (src[c] && (d + c + 1) < dstsize)
	{
		dst[d + c] = src[c];
		c++;
	}
	dst[d + c] = '\0';
	return (d + e);
}
