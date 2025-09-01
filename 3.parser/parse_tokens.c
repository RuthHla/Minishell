/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:35:52 by alandel           #+#    #+#             */
/*   Updated: 2025/09/01 12:32:52 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_redir(t_token *token_list)
{
	t_token	*tkn;
	t_token	*target;

	tkn = token_list;
	while (tkn)
	{
		if (!is_redir(tkn->type))
		{
			tkn = tkn->next;
			continue ;
		}
		target = tkn->next;
		if (!target)
		{
			fprintf(stderr,
				"bash: syntax error near unexpected token `newline'\n");
			return (0);
		}
		if (!(target->type == LITERAL || target->type == DOLLAR))
		{
			if (target->str && target->str[0])
				fprintf(stderr,
					"bash: syntax error near unexpected token `%s'\n",
					target->str);
			else
				fprintf(stderr,
					"bash: syntax error near unexpected token `newline'\n");
			return (0);
		}
		tkn = target->next;
	}
	return (1);
}

static int	check_pipe(t_token *token_list)
{
	t_token	*tkn;
	t_token	*target;

	tkn = token_list;
	if (!tkn)
		return (1);
	if (tkn->type == PIPE)
	{
		fprintf(stderr, "bash: syntax error near unexpected token '|'\n");
		return (0);
	}
	while (tkn)
	{
		target = tkn->next;
		if (tkn->type == PIPE)
		{
			if (!target || target->type == PIPE)
			{
				fprintf(stderr,
					"bash: syntax error near unexpected token '|'\n");
				return (0);
			}
		}
		tkn = target;
	}
	return (1);
}

int	parse_token(t_token *token_list)
{
	if (!check_redir(token_list))
		return (0);
	if (!check_pipe(token_list))
		return (0);
	return (1);
}
