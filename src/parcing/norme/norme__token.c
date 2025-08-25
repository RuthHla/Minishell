/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norme__token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 23:13:49 by adenny            #+#    #+#             */
/*   Updated: 2025/06/25 14:53:34 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	append_token(t_token **head, t_token **current, t_token *new_token)
{
	if (!*head)
		*head = new_token;
	else
		(*current)->next = new_token;
	*current = new_token;
	return (1);
}

t_token	*create_token_from_chars(t_character *chars, int word_id)
{
	t_token	*new_token;
	int		len;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	len = get_word_len(chars, word_id);
	new_token->str = build_token_string(chars, len);
	if (!new_token->str)
	{
		free(new_token);
		return (NULL);
	}
	if (chars->type == LITERAL)
		new_token->type = LITERAL;
	else if (chars->type == VARIABLE)
		new_token->type = VARIABLE;
	else if (len == 2)
		new_token->type = get_operator_token_type(chars);
	else if (len > 2)
		new_token->type = UNKNOWN;
	else
		new_token->type = chars->type;
	new_token->next = NULL;
	return (new_token);
}
