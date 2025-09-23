/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:47:13 by alandel           #+#    #+#             */
/*   Updated: 2025/09/11 14:21:09 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	detect_operator_type(t_character *c, t_character *n, t_type *type,
		size_t *len)
{
	*type = UNKNOWN;
	*len = 1;
	if (c->c == '|')
		*type = PIPE;
	else if (c->c == '<')
	{
		if (n && same_word(c, n) && n->c == '<')
		{
			*type = HEREDOC;
			*len = 2;
		}
		else
			*type = REDIR_IN;
	}
	else if (c->c == '>')
	{
		if (n && same_word(c, n) && n->c == '>')
		{
			*type = APPEND;
			*len = 2;
		}
		else
			*type = REDIR_OUT;
	}
}

int	is_expandable_dollar(t_character *dollar_char)
{
	t_character	*next;

	if (!dollar_char || dollar_char->c != '$')
		return (0);
	if (dollar_char->context == S_QUOTE)
		return (0);
	next = dollar_char->next;
	if (!next || !same_word(dollar_char, next))
		return (0);
	if (dollar_char->context != next->context)
		return (0);
	if (next->c == '?' || valid_variable_char(next->c))
		return (1);
	return (0);
}

int	word_has_special_variable(t_character *word_start)
{
	t_character	*current;
	t_character	*next;

	current = word_start;
	while (current && same_word(word_start, current))
	{
		if (current->c == '$' && current->context != S_QUOTE)
		{
			next = current->next;
			if (next && same_word(current, next)
				&& current->context == next->context && next->c == '?')
				return (1);
		}
		current = current->next;
	}
	return (0);
}

int	word_has_expandable_dollar(t_character *word_start)
{
	t_character	*current;
	t_character	*next;

	current = word_start;
	while (current && same_word(word_start, current))
	{
		if (is_expandable_dollar(current))
		{
			next = current->next;
			if (next && next->c != '?')
				return (1);
		}
		current = current->next;
	}
	return (0);
}

size_t	get_word_length(t_character *start)
{
	t_character	*cur;
	size_t		len;

	cur = start;
	len = 0;
	while (cur && same_word(start, cur) && !is_operator_type(cur->type))
	{
		len++;
		cur = cur->next;
	}
	return (len);
}
