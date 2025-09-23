/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:52:34 by adenny            #+#    #+#             */
/*   Updated: 2025/09/17 13:04:51 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_export_line(const char *entry)
{
	const char	*eq;
	size_t		ln;

	eq = ft_strchr(entry, '=');
	if (!eq)
	{
		write(STDOUT_FILENO, "declare -x ", sizeof("declare -x ") - 1);
		write(STDOUT_FILENO, entry, ft_strlen(entry));
		write(STDOUT_FILENO, "\n", 1);
		return ;
	}
	ln = (size_t)(eq - entry);
	write(STDOUT_FILENO, "declare -x ", sizeof("declare -x ") - 1);
	write(STDOUT_FILENO, entry, ln);
	write(STDOUT_FILENO, "=\"", sizeof("=\"") - 1);
	write(STDOUT_FILENO, eq + 1, ft_strlen(eq + 1));
	write(STDOUT_FILENO, "\"\n", sizeof("\"\n") - 1);
}

void	print_export_error(const char *s)
{
	write(STDERR_FILENO, "minishell: export: `", sizeof("minishell: export: `")
		- 1);
	if (s)
		write(STDERR_FILENO, s, ft_strlen(s));
	write(STDERR_FILENO, "': not a valid identifier\n",
		sizeof("': not a valid identifier\n") - 1);
}

int	handle_export_arg(const char *arg, t_shell *sh)
{
	const char	*eq;
	char		*name;

	if (!is_valid_ident_export(arg))
	{
		print_export_error(arg);
		return (0);
	}
	eq = ft_strchr(arg, '=');
	if (eq)
	{
		name = dup_n(arg, (size_t)(eq - arg));
		if (!name)
			return (0);
		if (!setenv_in_vec(&sh->env, name, eq + 1))
		{
			free(name);
			return (0);
		}
		free(name);
		return (1);
	}
	if (!setenv_in_vec(&sh->env, arg, ""))
		return (0);
	return (1);
}

int	builtin_export(t_command *cmd, t_shell *sh)
{
	char	**argv;
	int		idx;
	int		status;

	argv = build_export_argv(cmd->element);
	if (!argv)
		return (1);
	idx = 0;
	if (argv[0] && ft_strncmp(argv[0], "export", 6) == 0)
		idx = 1;
	if (!argv[idx])
	{
		status = print_all_exports(sh);
		free(argv);
		return (status);
	}
	status = process_export_args(argv, idx, sh);
	free(argv);
	return (status);
}
