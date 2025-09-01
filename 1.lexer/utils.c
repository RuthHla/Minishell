/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 16:46:07 by alandel           #+#    #+#             */
/*   Updated: 2025/09/01 16:35:00 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isspace(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

t_ctx	get_ctx_type(char c)
{
	if (c == 34)
		return (D_QUOTE);
	else if (c == 39)
		return (S_QUOTE);
	return (NONE);
}

t_type	get_character_type(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0'
			&& c <= '9') || c == '.' || c == '/' || c == '_' || c == '*'
		|| c == '[' || c == ']' || c == ',' || c == ':' || c == '+' || c == '='
		|| c == '%' || c == '#' || c == '@' || c == '-' || c == '?' || c == '!' || c == '&')
		return (LITERAL);
	else if (c == '|')
		return (PIPE);
	else if (c == '<')
		return (REDIR_IN);
	else if (c == '>')
		return (REDIR_OUT);
	else if (c == '$')
		return (DOLLAR);
	return (UNKNOWN);
}

int	handle_quote_context(char c, t_ctx *current_context)
{
	if ((c == 34 || c == 39) && (*current_context == NONE))
	{
		*current_context = get_ctx_type(c);
		return (1);
	}
	else if ((c == 34 || c == 39) && (*current_context != NONE))
	{
		if (c == 34 && *current_context == D_QUOTE)
		{
			*current_context = NONE;
			return (1);
		}
		else if (c == 39 && *current_context == S_QUOTE)
		{
			*current_context = NONE;
			return (1);
		}
	}
	return (0);
}
