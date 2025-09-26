/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_norme2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:24:35 by adenny            #+#    #+#             */
/*   Updated: 2025/09/26 14:49:34 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	exec_checked_path(char *path, char **argv, t_shell *sh,
		int need_free)
{
	int	e;

	if (is_directory(path))
	{
		errno = EISDIR;
		if (need_free)
			free(path);
		return (-1);
	}
	execve(path, argv, sh->env);
	if (need_free)
	{
		e = errno;
		free(path);
		errno = e;
	}
	return (-1);
}

int	exec_with_path(char **argv, t_shell *sh)
{
	char	*full;

	if (!argv || !argv[0])
	{
		errno = EINVAL;
		return (-1);
	}
	if (argv[0][0] == '\0')
	{
		errno = ENOENT;
		return (-1);
	}
	if (ft_strchr(argv[0], '/'))
		return (exec_checked_path((char *)argv[0], argv, sh, 0));
	full = resolve_in_path(argv[0], sh->env);
	if (full)
		return (exec_checked_path(full, argv, sh, 1));
	errno = ENOENT;
	return (-1);
}

int	is_simple_builtin_type(t_type_cmd c)
{
	return (c == T_ECHO || c == T_PWD || c == T_CD || c == T_EXPORT
		|| c == T_UNSET || c == T_ENV || c == T_EXIT);
}

int	is_builtin_cmd(t_type_cmd c)
{
	return (is_simple_builtin_type(c));
}

int	exec_builtin(t_command *cmd, t_shell *sh, t_all *all)
{
	if (cmd->cmd == T_ECHO)
		return (builtin_echo(cmd, sh));
	if (cmd->cmd == T_PWD)
		return (builtin_pwd(cmd, sh));
	if (cmd->cmd == T_CD)
		return (builtin_cd(cmd, sh));
	if (cmd->cmd == T_EXPORT)
		return (builtin_export(cmd, sh));
	if (cmd->cmd == T_UNSET)
		return (builtin_unset(cmd, sh));
	if (cmd->cmd == T_ENV)
		return (builtin_env(cmd, sh));
	if (cmd->cmd == T_EXIT)
		return (builtin_exit(cmd, sh, all));
	return (127);
}
