/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:31:36 by alandel           #+#    #+#             */
/*   Updated: 2025/09/17 11:09:20 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	process_special_tokens(t_token **token_list, t_command **current)
{
	if (is_redir((*token_list)->type))
		return (add_redir(token_list, current));
	if ((*token_list)->type == LITERAL || (*token_list)->type == DOLLAR
		|| (*token_list)->type == SPECIAL_VARIABLE)
		return (handle_cmd_or_arg(token_list, current));
	if ((*token_list)->type == PIPE)
		return (handle_pipe(token_list, current));
	return (-1);
}

int	save_all(t_command *cmd, t_token *token_list)
{
	t_command	*current;
	int			result;

	current = cmd;
	while (token_list)
	{
		result = process_special_tokens(&token_list, &current);
		if (result == 0)
			return (0);
		if (result == 1)
			continue ;
		if (!add_argument(current, token_list->type, token_list->str))
			return (0);
		token_list = token_list->next;
	}
	return (1);
}

t_command	*create_new_command(void)
{
	t_command	*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->cmd = CMD_NONE;
	return (cmd);
}

t_command	*init_struct_globale(t_token *token_list)
{
	t_command	*cmd;

	cmd = create_new_command();
	if (!cmd)
		return (NULL);
	if (!parse_token(token_list))
	{
		cleanup(cmd);
		return (NULL);
	}
	if (!save_all(cmd, token_list))
	{
		cleanup(cmd);
		return (NULL);
	}
	return (cmd);
}
