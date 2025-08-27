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
	node->word_id = count_word;
	if(ch == '$' && context == D_QUOTE)
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

// t_character *build_char_list(char *line)
// {
// 	t_ctx		current_context;
// 	t_character	*head;
// 	t_character	*tail;
// 	int			skip_char;
// 	int			count_word;
// 	int			i;
// 	t_character	*new_node;

// 	current_context = NONE;
// 	head = NULL;
// 	tail = NULL;
// 	skip_char = 0;
// 	count_word = 0;
// 	i = 0;
// 	while (line[i] != '\0')
// 	{
// 		skip_char = handle_quote_context(line[i], &current_context);
// 		if (ft_isspace(line[i]) && current_context == NONE)
// 		{
// 			skip_char = 1;
// 			if (tail && tail->word_id == count_word)
// 				count_word++;
// 		}
// 		if (line[i] == '\\' || line[i] == ';') // remplacer par check_invalid_char
// 		{
// 			fprintf(stderr, "Find an invalid character\n");
// 			free_character_list(head);
// 			return (NULL);
// 		}
// 		if (!skip_char)
// 		{
// 			new_node = init_node(line[i], current_context, count_word, tail);
// 			if (!new_node)
// 			{
// 				free_character_list(head);
// 				return (NULL);
// 			}
// 			if (!head)
// 				head = new_node;
// 			tail = new_node;
// 		}
// 		skip_char = 0;
// 		i++;
// 	}
// 	if (check_oprhan_quote(head, current_context))
// 		return (NULL);
// 	return (head);
// }

static int process_space(char ch, t_ctx ctx, int *count_word, t_character *tail)
{
    if (ft_isspace(ch) && ctx == NONE)
    {
        if (tail && tail->word_id == *count_word)
            (*count_word)++;
        return 1;
    }
    return 0;
}

static int append_char(t_character **head, t_character **tail,
                       char ch, t_ctx ctx, int count_word)
{
    t_character *new = init_node(ch, ctx, count_word, *tail);
    if (!new)
    {
        free_character_list(*head);
        return 0;
    }
    if (!*head)
        *head = new;
    *tail = new;
    return 1;
}

t_character *build_char_list(char *line)
{
    t_ctx        ctx = NONE;
    t_character *head = NULL;
    t_character *tail = NULL;
    int          count_word = 0;
    int          i = 0;

    while (line[i])
    {
        int skip = handle_quote_context(line[i], &ctx);
        if (process_space(line[i], ctx, &count_word, tail))
            skip = 1;
        if (line[i] == '\\' || line[i] == ';')
        {
            fprintf(stderr, "Find an invalid character\n");
            free_character_list(head);
            return NULL;
        }
        if (!skip && !append_char(&head, &tail, line[i], ctx, count_word))
            return NULL;
        i++;
    }
    if (check_oprhan_quote(head, ctx))
        return NULL;
    return head;
}
