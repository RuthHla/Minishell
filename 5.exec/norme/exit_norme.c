/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_norme.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:40:45 by adenny            #+#    #+#             */
/*   Updated: 2025/09/16 13:51:05 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_numeric_word(const char *s)
{
	const char	*p = s;

	if (!s || !*s)
		return (0);
	if (*p == '+' || *p == '-')
		p++;
	if (!*p)
		return (0);
	while (*p)
	{
		if (*p < '0' || *p > '9')
			return (0);
		p++;
	}
	return (1);
}

char	to_exit_u8(const char *s)
{
	int	v;

	v = ft_atoi(s);
	return ((unsigned char)v);
}
