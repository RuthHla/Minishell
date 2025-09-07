/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:36:13 by alandel           #+#    #+#             */
/*   Updated: 2025/09/01 17:06:28 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	*create_new_command(void)
{
	t_command	*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->cmd = CMD_NONE;
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
