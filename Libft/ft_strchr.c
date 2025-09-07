/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 22:48:52 by adenny            #+#    #+#             */
/*   Updated: 2024/11/18 22:48:57 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	int	k;

	k = 0;
	while (str[k])
	{
		if (str[k] == (char)c)
			return ((char *)&str[k]);
		k++;
	}
	if ((char)c == '\0')
		return ((char *)&str[k]);
	return (NULL);
}
