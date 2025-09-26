/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_norme.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:21:41 by adenny            #+#    #+#             */
/*   Updated: 2025/09/26 14:10:15 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	print_errno_detail(const char *shown, int e)
{
	if (!shown)
		shown = "(null)";
	if (e == EISDIR)
		print2_err(shown, ": Is a directory\n");
	else if (e == ENOENT)
		print2_err(shown, ": No such file or directory\n");
	else if (e == EACCES)
		print2_err(shown, ": Permission denied\n");
	else if (e == ENOTDIR)
		print2_err(shown, ": Not a directory\n");
	else if (e == ENOEXEC)
		print2_err(shown, ": Exec format error\n");
	else
	{
		print2_err(shown, ": ");
		putstr_err(strerror(e));
		putstr_err("\n");
	}
}

static void	print_no_path_cmd_error(const char *argv0, int e)
{
	if (e == ENOENT)
	{
		ft_putstr_fd("minishell: ", 2);
		print2_err(argv0, " : command not found\n");
	}
	else
	{
		print2_err(argv0, ": ");
		putstr_err(strerror(e));
		putstr_err("\n");
	}
}

void	print_exec_error(const char *path, const char *argv0, int e)
{
	const char	*shown;

	if (!argv0 || argv0[0] == '\0')
	{
		print_error(": command not found");
		e = ENOENT;
		return ;
	}
	shown = argv0;
	if (path)
		shown = path;
	if (argv0 && ft_strchr(argv0, '/') == NULL)
	{
		print_no_path_cmd_error(argv0, e);
		return ;
	}
	print_errno_detail(shown, e);
}

static char	*try_path_dirs(char *copy, const char *cmd)
{
	char	*dir;
	char	*sep;
	char	*candidate;

	dir = copy;
	while (dir)
	{
		sep = ft_strchr(dir, ':');
		if (sep)
			*sep = '\0';
		if (*dir)
		{
			candidate = join_path(dir, cmd);
			if (!candidate)
				return (NULL);
			if (access(candidate, X_OK) == 0)
				return (candidate);
			free(candidate);
		}
		if (!sep)
			break ;
		dir = sep + 1;
	}
	return (NULL);
}

char	*resolve_in_path(const char *cmd, char **env)
{
	char	*path;
	char	*copy;
	char	*res;

	path = find_variable_in_env(env, "PATH");
	if (!path || !*path)
		return (NULL);
	copy = ft_strdup(path);
	if (!copy)
		return (NULL);
	res = try_path_dirs(copy, cmd);
	free(copy);
	return (res);
}
