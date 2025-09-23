/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:36:20 by alandel           #+#    #+#             */
/*   Updated: 2025/09/17 11:50:44 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_cmd(t_command *cmd, char *str)
{
	if (cmd->cmd == CMD_NONE)
	{
		cmd->cmd = identify_builtin(str);
		return (1);
	}
	return (0);
}

int	add_argument(t_command *cmd, t_type type, const char *str)
{
	t_element	*element;

	if (!cmd || !str)
		return (0);
	element = create_element_arg(type, str);
	if (!element)
		return (0);
	add_element_to_command(cmd, element);
	cmd->nb_args++;
	return (1);
}

int	add_redir(t_token **token_list, t_command **current)
{
	t_token		*redir;
	t_token		*target;
	t_element	*element;

	if (!token_list || !*token_list || !current || !*current)
		return (0);
	redir = *token_list;
	target = redir->next;
	if (!target)
		return (0);
	element = create_element_redir(redir->type, target->str, target->type);
	if (!element)
		return (0);
	add_element_to_command(*current, element);
	*token_list = target->next;
	return (1);
}

int	handle_pipe(t_token **token_list, t_command **current)
{
	t_command	*new_node;

	new_node = NULL;
	if ((*token_list)->type == PIPE)
	{
		new_node = create_new_command();
		if (!new_node)
		{
			print_error("EXIT FAILURE");
			return (0);
		}
		(*current)->has_pipe_out = 1;
		(*current)->next = new_node;
		new_node->previous = *current;
		(*current) = new_node;
		*token_list = (*token_list)->next;
		return (1);
	}
	return (0);
}

int	handle_cmd_or_arg(t_token **token_list, t_command **current)
{
	t_type_cmd	b;

	if ((*current)->cmd == CMD_NONE)
	{
		b = identify_builtin((*token_list)->str);
		(*current)->cmd = b;
	}
	if (!add_argument(*current, (*token_list)->type, (*token_list)->str))
		return (0);
	*token_list = (*token_list)->next;
	return (1);
}
