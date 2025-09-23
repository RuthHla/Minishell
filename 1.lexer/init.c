/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 16:46:00 by alandel           #+#    #+#             */
/*   Updated: 2025/09/17 13:14:02 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_character	*init_node(char ch, t_ctx context, t_character *tail)
{
	t_character	*node;

	node = malloc(sizeof(t_character));
	if (!node)
		return (NULL);
	node->c = ch;
	node->word_id = 0;
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

int	check_empty_string(t_ctx ctx, char next_c, char next_next_c)
{
	t_ctx	next_ctx;

	next_ctx = get_ctx_type(next_c);
	if (ctx == next_ctx && (is_operator_char(next_next_c)
			|| ft_isspace(next_next_c) || next_next_c == '\0'))
		return (1);
	return (0);
}

int	check_oprhan_quote(t_character *head, t_ctx current_context)
{
	if (current_context != NONE)
	{
		print_error("Find an orphan quote");
		free_character_list(head);
		return (1);
	}
	return (0);
}
