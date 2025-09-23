/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:18:07 by adenny            #+#    #+#             */
/*   Updated: 2025/09/17 13:46:33 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	print_unset_error(const char *name)
{
	write(STDERR_FILENO, "minishell: unset: `",
		ft_strlen("minishell: unset: `"));
	if (name)
		write(STDERR_FILENO, name, ft_strlen(name));
	write(STDERR_FILENO, "': not a valid identifier\n",
		ft_strlen("': not a valid identifier\n"));
}

static int	process_unset_args(char **args, t_shell *sh)
{
	int	status;

	status = 0;
	while (*args)
	{
		if (!is_valid_ident(*args))
		{
			print_unset_error(*args);
			status = 1;
		}
		else
			remove_env_var(&sh->env, *args);
		args++;
	}
	return (status);
}

int	builtin_unset(t_command *cmd, t_shell *sh)
{
	size_t	argc;
	char	**argv;
	char	**args;
	int		status;

	argc = count_args(cmd->element);
	argv = (char **)ft_calloc(argc + 1, sizeof(char *));
	if (!argv)
		return (1);
	fill_argv(cmd->element, argv);
	args = argv;
	if (args[0] && ft_strncmp(args[0], "unset", 5) == 0)
		args++;
	status = process_unset_args(args, sh);
	free(argv);
	return (status);
}
