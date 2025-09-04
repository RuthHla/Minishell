/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 16:46:00 by alandel           #+#    #+#             */
/*   Updated: 2025/09/04 10:29:36 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// 1. Un caractère blanc (IFS) en dehors des quotes
// 2. Un opérateur
// 3. La fin d'un opérateur
// 4. Un quote non échappé qui ouvre/ferme

#include "../minishell.h"

static t_character	*init_node(char ch, t_ctx context, int count_word,
		t_character *tail)
{
	t_character	*node;

	node = malloc(sizeof(t_character));
	if (!node)
		return (NULL);
	node->c = ch;
	node->word_id = count_word;
	if (context == S_QUOTE)
		node->type = LITERAL;
	else if (context == D_QUOTE)
	{
		if (ch == '$')
			node->type = DOLLAR;
		else
			node->type = LITERAL;
	}
	else
		node->type = get_character_type(ch);
	node->context = context;
	node->next = NULL;
	node->prev = tail;
	if (tail)
		tail->next = node;
	return (node);
}

static int	check_oprhan_quote(t_character *head, t_ctx current_context)
{
	if (current_context != NONE)
	{
		fprintf(stderr, "Find an orphan quote\n");
		free_character_list(head);
		return (1);
	}
	return (0);
}

static int	append_char(t_character **head, t_character **tail, char ch,
		t_ctx ctx, int count_word)
{
	t_character	*new;

	new = init_node(ch, ctx, count_word, *tail);
	if (!new)
	{
		free_character_list(*head);
		return (0);
	}
	if (!*head)
		*head = new;
	*tail = new;
	return (1);
}

t_character	*build_char_list(char *line)
{
	t_ctx		ctx;
	t_character	*head;
	t_character	*tail;
	int			word;
	int			i;

	ctx = NONE;
	head = NULL;
	tail = NULL;
	word = 0;
	i = 0;
	while (line[i])
	{
		if (handle_quote_context(line[i], &ctx))
		{
			i++;
			continue ;
		}
		else if (ctx == NONE && ft_isspace(line[i]))
		{
			if (tail && tail->word_id == word)
				word++;
			i++;
			continue ;
		}
		else if (ctx == NONE && is_operator_char(line[i]))
		{
			if (tail && tail->word_id == word && !is_operator_char(tail->c))
				word++;
			else if (tail && tail->word_id == word && is_operator_char(tail->c)
				&& tail->c != line[i])
				word++;
			if (!append_char(&head, &tail, line[i], ctx, word))
				return (NULL);
		}
		else
		{
			if (tail && tail->word_id == word && tail->context == NONE
				&& is_operator_char(tail->c))
				word++;
			if (!append_char(&head, &tail, line[i], ctx, word))
				return (NULL);
		}
		i++;
	}
	if (check_oprhan_quote(head, ctx))
		return (NULL);
	return (head);
}
