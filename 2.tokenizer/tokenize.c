/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:47:13 by alandel           #+#    #+#             */
/*   Updated: 2025/09/11 14:48:06 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_null_word(t_token **head, t_token **tail,
		t_character **char_list, t_character *current)
{
	t_token	*tok;

	if (!current || !current->next)
		return (0);
	if (!current->word_id || !current->next->word_id)
		return (0);
	tok = new_token(LITERAL, 0);
	if (!tok)
		return (0);
	append_token(head, tail, tok);
	*char_list = (*char_list)->next;
	return (1);
}

static void	fill_word_token(t_token *tok, t_character *start, size_t len)
{
	t_character	*cur;
	size_t		i;

	cur = start;
	i = 0;
	while (cur && same_word(start, cur) && !is_operator_type(cur->type))
	{
		tok->str[i] = cur->c;
		i++;
		cur = cur->next;
	}
	tok->str[len] = '\0';
}

static int	create_word_token(t_token **head, t_token **tail,
		t_character **char_list)
{
	t_character	*start;
	t_character	*cur;
	size_t		len;
	t_type		type;
	t_token		*tok;

	start = *char_list;
	len = get_word_length(start);
	if (len == 0 && handle_null_word(head, tail, char_list, start))
		return (1);
	type = LITERAL;
	if (word_has_special_variable(start))
		type = SPECIAL_VARIABLE;
	else if (word_has_expandable_dollar(start))
		type = DOLLAR;
	tok = new_token(type, len);
	if (!tok)
		return (0);
	fill_word_token(tok, start, len);
	append_token(head, tail, tok);
	cur = start;
	while (cur && same_word(start, cur) && !is_operator_type(cur->type))
		cur = cur->next;
	*char_list = cur;
	return (1);
}

static int	create_operator_token(t_token **head, t_token **tail,
		t_character **pos)
{
	t_character	*c;
	t_character	*n;
	t_type		type;
	size_t		len;
	t_token		*tok;

	c = *pos;
	if (!c)
		return (0);
	n = c->next;
	detect_operator_type(c, n, &type, &len);
	tok = new_token(type, len);
	if (!tok)
		return (0);
	tok->str[0] = c->c;
	if (len == 2 && n)
		tok->str[1] = n->c;
	tok->str[len] = '\0';
	append_token(head, tail, tok);
	if (len == 2 && n)
		*pos = n->next;
	else
		*pos = c->next;
	return (1);
}

t_token	*build_token_list(t_character *char_list)
{
	t_token	*head;
	t_token	*tail;

	head = NULL;
	tail = NULL;
	while (char_list)
	{
		if (is_operator_type(char_list->type))
		{
			if (!create_operator_token(&head, &tail, &char_list))
				return (free_token_list(head), NULL);
		}
		else
		{
			if (!create_word_token(&head, &tail, &char_list))
				return (free_token_list(head), NULL);
		}
	}
	return (head);
}
