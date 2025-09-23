/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 09:57:52 by adenny            #+#    #+#             */
/*   Updated: 2025/09/17 12:51:28 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	print_env(char **env)
{
	size_t	i;

	i = 0;
	while (env && env[i])
	{
		if (ft_strchr(env[i], '='))
		{
			write(STDOUT_FILENO, env[i], ft_strlen(env[i]));
			write(STDOUT_FILENO, "\n", 1);
		}
		i++;
	}
}

int	builtin_env(t_command *cmd, t_shell *sh)
{
	size_t	argc;
	char	**argv;
	int		idx;

	argc = count_args(cmd->element);
	argv = (char **)ft_calloc(argc + 1, sizeof(char *));
	if (!argv)
		return (1);
	fill_argv(cmd->element, argv);
	idx = 0;
	if (argv[0] && ft_strncmp(argv[0], "env", ft_strlen(argv[0])) == 0)
		idx = 1;
	if (argv[idx])
	{
		write(STDERR_FILENO, "minishell: env: too many arguments\n",
			ft_strlen("minishell: env: too many arguments\n"));
		free(argv);
		return (1);
	}
	print_env(sh->env);
	free(argv);
	return (0);
}
