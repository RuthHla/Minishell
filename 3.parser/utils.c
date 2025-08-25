/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:36:06 by alandel           #+#    #+#             */
/*   Updated: 2025/06/26 13:00:39 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*lst_last_node(void *head)
{
	t_token	*curr;

	curr = (t_token *)head;
	if (!curr)
		return (NULL);
	while (curr->next)
		curr = curr->next;
	return (curr);
}

int	is_redir(t_type type)
{
	if (type == HEREDOC || type == APPEND || type == REDIR_IN
		|| type == REDIR_OUT)
		return (1);
	return (0);
}

int	is_operator(t_type type)
{
	if (type == PIPE || type == OR || type == AMPERSAND || type == AND
		|| type == REDIR_IN || type == HEREDOC || type == REDIR_OUT
		|| type == APPEND)
		return (1);
	return (0);
}

int	is_command(t_type_cmd type)
{
	if (type == T_ECHO || type == T_CD || type == T_PWD || type == T_EXPORT
		|| type == T_UNSET || type == T_ENV || type == T_EXIT)
		return (1);
	return (0);
}

t_type_cmd	identify_builtin(const char *str)
{
	if (!str)
		return (NOT_BUILTIN);
	if (strcmp(str, "echo") == 0)
		return (T_ECHO);
	if (strcmp(str, "cd") == 0)
		return (T_CD);
	if (strcmp(str, "pwd") == 0)
		return (T_PWD);
	if (strcmp(str, "export") == 0)
		return (T_EXPORT);
	if (strcmp(str, "unset") == 0)
		return (T_UNSET);
	if (strcmp(str, "env") == 0)
		return (T_ENV);
	if (strcmp(str, "exit") == 0)
		return (T_EXIT);
	return (NOT_BUILTIN);
}
