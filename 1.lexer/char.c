/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 16:46:00 by alandel           #+#    #+#             */
/*   Updated: 2025/09/02 08:59:49 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if (ch == '$' && context == D_QUOTE)
		node->type = DOLLAR;
	else if (context != NONE)
		node->type = LITERAL;
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

static int should_increment_word(t_ctx old_ctx, t_ctx new_ctx, char ch, t_character *tail)
{
	if (old_ctx != NONE && new_ctx == NONE && !ft_isspace(ch))
		return (1);
	if (old_ctx == NONE && new_ctx != NONE && tail && tail->word_id >= 0)
		return (1);
	return (0);
}


static int	process_space(char ch, t_ctx ctx, int *count_word,
		t_character *tail)
{
	if (ft_isspace(ch) && ctx == NONE)
	{
		if (tail && tail->word_id == *count_word)
			(*count_word)++;
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
	t_ctx		old_ctx;
	t_character	*head;
	t_character	*tail;
	int			count_word;
	int			i;
	int			skip;

	ctx = NONE;
	old_ctx = NONE;
	head = NULL;
	tail = NULL;
	count_word = 0;
	i = 0;
	while (line[i])
	{
		old_ctx = ctx;
		skip = handle_quote_context(line[i], &ctx);
		if (should_increment_word(old_ctx, ctx, line[i], tail))
			count_word++;
		if (process_space(line[i], ctx, &count_word, tail))
			skip = 1;
		if (ctx == NONE && (line[i] == '\\' || line[i] == ';'))
		{
			fprintf(stderr, "Find an invalid character\n");
			free_character_list(head);
			return (NULL);
		}
		if (!skip && !append_char(&head, &tail, line[i], ctx, count_word))
			return (NULL);
		i++;
	}
	if (check_oprhan_quote(head, ctx))
		return (NULL);
	return (head);
}
