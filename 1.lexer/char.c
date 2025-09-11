/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 16:46:00 by alandel           #+#    #+#             */
/*   Updated: 2025/09/11 15:18:05 by alandel          ###   ########.fr       */
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

static int	check_empty_string(t_ctx ctx, char next_c, char next_next_c)
{
	t_ctx	next_ctx;

	next_ctx = get_ctx_type(next_c);
	if (ctx == next_ctx && (is_operator_char(next_next_c)
			|| ft_isspace(next_next_c) || next_next_c == '\0'))
		return (1);
	return (0);
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
			if (ctx != NONE && line[i + 1] != '\0')
			{
				if (check_empty_string(ctx, line[i + 1], line[i + 2]))
				{
					if (!append_char(&head, &tail, '\0', ctx, word))
						return (NULL);
					i += 2;
					ctx = NONE;
					continue ;
				}
			}
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

// t_character	*build_char_list(char *line)
// {
// 	t_ctx		ctx;
// 	t_character	*head;
// 	t_character	*tail;
// 	int			word;
// 	int			i;

// 	ctx = NONE;
// 	head = NULL;
// 	tail = NULL;
// 	word = 0;
// 	i = 0;
// 	while (line[i])
// 	{
// 		if (process_quote_block(line, &i, &ctx, &head, &tail, word))
// 			continue ;
// 		if (process_space(line[i], ctx, &i, tail, word))
// 			word++;
// 		else if (!process_token(line[i], ctx, &word, &head, &tail))
// 			return (NULL);
// 		i++;
// 	}
// 	if (check_oprhan_quote(head, ctx))
// 		return (NULL);
// 	return (head);
// }
