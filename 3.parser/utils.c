/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:36:06 by alandel           #+#    #+#             */
/*   Updated: 2025/09/17 13:04:30 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_element	*lst_last_node(t_element *head)
{
	t_element	*cur;

	cur = head;
	if (!cur)
		return (NULL);
	while (cur->next)
		cur = cur->next;
	return (cur);
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
	if (type == PIPE || type == REDIR_IN || type == HEREDOC || type == REDIR_OUT
		|| type == APPEND || type == DOLLAR || type == SPECIAL_VARIABLE)
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
	int	len;

	if (!str)
		return (NOT_BUILTIN);
	len = ft_strlen(str);
	if (len == 4 && (ft_strncmp(str, "echo", len) == 0))
		return (T_ECHO);
	if (len == 2 && (ft_strncmp(str, "cd", len) == 0))
		return (T_CD);
	if (len == 3 && (ft_strncmp(str, "pwd", len) == 0))
		return (T_PWD);
	if (len == 6 && (ft_strncmp(str, "export", len) == 0))
		return (T_EXPORT);
	if (len == 5 && (ft_strncmp(str, "unset", len) == 0))
		return (T_UNSET);
	if (len == 3 && (ft_strncmp(str, "env", len) == 0))
		return (T_ENV);
	if (len == 4 && (ft_strncmp(str, "exit", len) == 0))
		return (T_EXIT);
	return (NOT_BUILTIN);
}
