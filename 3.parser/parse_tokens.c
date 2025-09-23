/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:35:52 by alandel           #+#    #+#             */
/*   Updated: 2025/09/17 13:02:27 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	validate_redir_target(t_token *redir, t_token **next)
{
	t_token	*target;

	target = redir->next;
	if (!target)
	{
		print_error("syntax error near unexpected token `newline'");
		return (0);
	}
	if (!(target->type == LITERAL || target->type == DOLLAR
			|| target->type == SPECIAL_VARIABLE))
	{
		if (target->str && target->str[0])
			print_syntax_error(target->str);
		else
			print_error("syntax error near unexpected token `newline'");
		return (0);
	}
	*next = target->next;
	return (1);
}

static int	check_redir(t_token *token_list)
{
	t_token	*tkn;

	tkn = token_list;
	while (tkn)
	{
		if (!is_redir(tkn->type))
		{
			tkn = tkn->next;
			continue ;
		}
		if (!validate_redir_target(tkn, &tkn))
			return (0);
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
		print_error("syntax error near unexpected token '|'");
		return (0);
	}
	while (tkn)
	{
		target = tkn->next;
		if (tkn->type == PIPE)
		{
			if (!target || target->type == PIPE)
			{
				print_error("syntax error near unexpected token '|'");
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
