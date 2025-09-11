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

static int	is_expandable_dollar(t_character *dollar_char)
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

static int	word_has_special_variable(t_character *word_start)
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

static int	word_has_expandable_dollar(t_character *word_start)
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

int	create_word_token(t_token **head, t_token **tail,
		t_character **char_list)
{
	t_character	*word_start;
	t_character	*current;
	size_t		len;
	t_type		token_type;
	t_token		*tok;
	size_t		i;

	word_start = *char_list;
	current = word_start;
	len = 0;
	token_type = LITERAL;
	i = 0;
	// calculer la len de la string de notre token
	while (current && same_word(word_start, current)
		&& !is_operator_type(current->type))
	{
		len++;
		current = current->next;
	}
	// si jai une string null alors je cree un token string null (si pas dans le meme mot)
	if (len == 0 && (current->word_id && current->next->word_id))
	{
		tok = new_token(LITERAL, 0);
		if (!tok)
			return (0);
		append_token(head, tail, tok);
		*char_list = (*char_list)->next;
		return (1);
	}
		
	if (word_has_special_variable(word_start))
		token_type = SPECIAL_VARIABLE;
	else if (word_has_expandable_dollar(word_start))
		token_type = DOLLAR;
	// creation du nouveau token
	tok = new_token(token_type, len);
	if (!tok)
		return (0);
	current = word_start;
	i = 0;
	// remplir str du nouveau token
	while (current && same_word(word_start, current)
		&& !is_operator_type(current->type))
	{
		tok->str[i++] = current->c;
		current = current->next;
	}
	tok->str[len] = '\0';
	// ajouter token a la liste
	append_token(head, tail, tok);
	*char_list = current;
	return (1);
}


int	create_operator_token(t_token **h, t_token **t, t_character **p)
{
	t_character	*c;
	t_type		type;
	size_t		len;
	t_token		*tok;
	t_character	*n;

	c = *p;
	n = (c ? c->next : NULL);
	type = UNKNOWN;
	len = 1;
	if (c->c == '|')
		type = PIPE;
	else if (c->c == '<')
	{
		if (n && same_word(c, n) && n->c == '<')
		{
			type = HEREDOC;
			len = 2;
		}
		else
			type = REDIR_IN;
	}
	else if (c->c == '>')
	{
		if (n && same_word(c, n) && n->c == '>')
		{
			type = APPEND;
			len = 2;
		}
		else
			type = REDIR_OUT;
	}
	tok = new_token(type, len);
	if (!tok)
		return (0);
	tok->str[0] = c->c;
	if (len == 2 && n)
		tok->str[1] = n->c;
	tok->str[len] = '\0';
	append_token(h, t, tok);
	if (len == 2 && n)
		*p = n->next;
	else
		*p = c->next;
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
