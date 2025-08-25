/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 16:46:00 by alandel           #+#    #+#             */
/*   Updated: 2025/08/25 17:18:06 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_character_list(t_character *head)
{
	t_character	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head);
		head = tmp;
	}
}

t_character	*init_node(char ch, t_ctx context, int count_word,
		t_character *tail)
{
	t_character	*node;

	node = malloc(sizeof(t_character));
	if (!node)
		return (NULL);
	node->c = ch;
	node->word = count_word;
	if (context != NONE)
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

int check_oprhan_quote(t_character *head, t_ctx current_context)
{
	if (current_context != NONE)
	{
		fprintf(stderr, "Find an orphan quote\n");
		free_character_list(head);
		return 1;
	}
	return 0;
}

t_character *build_lexer_list(char *line)
{
	t_ctx		current_context;
	t_character	*head;
	t_character	*tail;
	int			skip_char;
	int			count_word;
	int			i;
	t_character	*new_node;

	current_context = NONE;
	head = NULL;
	tail = NULL;
	skip_char = 0;
	count_word = 0;
	i = 0;
	while (line[i] != '\0')
	{
		skip_char = handle_quote_context(line[i], &current_context);
		if (ft_isspace(line[i]) && current_context == NONE)
		{
			skip_char = 1;
			if (tail && tail->word == count_word)
				count_word++;
		}
		if (line[i] == '\\' || line[i] == ';')
		{
			fprintf(stderr, "Find an invalid character\n");
			free_character_list(head);
			return (NULL);
		}
		if (!skip_char)
		{
			new_node = init_node(line[i], current_context, count_word, tail);
			if (!new_node)
			{
				free_character_list(head);
				return (NULL);
			}
			if (!head)
				head = new_node;
			tail = new_node;
		}
		skip_char = 0;
		i++;
	}
	if (check_oprhan_quote(head, current_context))
		return (NULL);
	return (head);
}
