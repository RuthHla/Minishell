/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:36:20 by alandel           #+#    #+#             */
/*   Updated: 2025/09/02 09:45:34 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Proteger fonction -> utiliser une variuable plutit que la liste elle meme
// + verif tj check liste non null

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

static int	add_redir(t_token **token_list, t_command **current)
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

static int	handle_pipe(t_token **token_list, t_command **current)
{
	t_command	*new_node;

	new_node = NULL;
	if ((*token_list)->type == PIPE)
	{
		new_node = create_new_command();
		(*current)->has_pipe_out = 1;
		(*current)->next = new_node;
		new_node->previous = *current;
		(*current) = new_node;
		*token_list = (*token_list)->next;
		return (1);
	}
	return (0);
}

static void	handle_cmd_or_arg(t_token **token_list, t_command **current,
		char **line)
{
	if ((*current)->cmd == CMD_NONE)
	{
		if (!add_cmd(*current, (*token_list)->str))
			cleanall_exit(*current, (*token_list), line);
		// pas bon car fait token_list-next juste apres
	}
	else
	{
		if (!add_argument(*current, (*token_list)->type, (*token_list)->str))
			cleanall_exit(*current, *token_list, line);
		// pas bon car fait token_list-next juste apres
	}
	*token_list = (*token_list)->next;
}

int	save_all(t_command *cmd, t_token *token_list, char **line)
{
	t_command	*current;

	current = cmd;
	while (token_list)
	{
		if (is_redir(token_list->type))
		{
			if (!add_redir(&token_list, &current))
			{
				cleanall_exit(current, token_list, line);
				return (0);
			}
			continue ;
		}
		else if (token_list->type == LITERAL || token_list->type == DOLLAR
			|| token_list->type == SPECIAL_VARIABLE)
			handle_cmd_or_arg(&token_list, &current, line);
		
		else if (token_list->type == PIPE)
		{
			if (!handle_pipe(&token_list, &current))
			{
				cleanall_exit(current, token_list, line);
				return (0);
			}
		}
		else
		{
			if (!add_argument(current, token_list->type, token_list->str))
			{
				cleanall_exit(current, token_list, line);
				return (0);
			}
			token_list = token_list->next;
			continue ;
		}
	}
	return (1);
}
