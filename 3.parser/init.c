/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:36:13 by alandel           #+#    #+#             */
/*   Updated: 2025/06/26 11:36:14 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	*create_new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->cmd = CMD_NONE;
	cmd->args = NULL;
	cmd->nb_args = 0;
	cmd->redirs = NULL;
	cmd->has_pipe_out = 0;
	cmd->next = NULL;
	cmd->previous = NULL;
	return (cmd);
}

t_command	*init_struct_globale(t_token *token_list, char **line)
{
	t_command	*cmd;

	cmd = create_new_command();
	if (!cmd)
		return (NULL);
	if (!parse_token(token_list))
		return NULL;
	if (!save_all(cmd, token_list, line))
		return NULL;
	return (cmd);
}
