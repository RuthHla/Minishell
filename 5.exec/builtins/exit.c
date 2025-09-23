/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:30:31 by adenny            #+#    #+#             */
/*   Updated: 2025/09/16 14:56:11 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exit_no_arg(char **argv, t_shell *sh, t_all *all)
{
	free(argv);
	free_env(sh);
	cleanall(all->char_list, all->token_list, all->command_list);
	rl_clear_history();
	exit(sh->last_exit);
}

static void	exit_numeric_error(char **argv, int idx, t_shell *sh, t_all *all)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(argv[idx], STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	free(argv);
	free_env(sh);
	cleanall(all->char_list, all->token_list, all->command_list);
	rl_clear_history();
	exit(2);
}

static int	exit_too_many_args(char **argv, t_shell *sh, t_all *all)
{
	ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
	free(argv);
	free_env(sh);
	cleanall(all->char_list, all->token_list, all->command_list);
	rl_clear_history();
	return (1);
}

static char	**collect_args(t_command *cmd, size_t *argc)
{
	t_element	*e;
	char		**argv;
	size_t		i;

	*argc = 0;
	e = cmd->element;
	while (e)
	{
		if (e->kind == ARG)
			(*argc)++;
		e = e->next;
	}
	argv = (char **)ft_calloc(*argc + 1, sizeof(char *));
	if (!argv)
		return (NULL);
	i = 0;
	e = cmd->element;
	while (e)
	{
		if (e->kind == ARG)
			argv[i++] = e->u_.arg->str;
		e = e->next;
	}
	argv[i] = NULL;
	return (argv);
}

int	builtin_exit(t_command *cmd, t_shell *sh, t_all *all)
{
	size_t			argc;
	char			**argv;
	int				idx;
	unsigned char	code;

	argv = collect_args(cmd, &argc);
	if (!argv)
		return (1);
	idx = 0;
	if (argv[0] && !ft_strncmp(argv[0], "exit", ft_strlen(argv[0]) + 1))
		idx = 1;
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (!argv[idx])
		exit_no_arg(argv, sh, all);
	if (!is_numeric_word(argv[idx]))
		exit_numeric_error(argv, idx, sh, all);
	if (argv[idx + 1])
		return (exit_too_many_args(argv, sh, all));
	code = to_exit_u8(argv[idx]);
	free(argv);
	free_env(sh);
	cleanall(all->char_list, all->token_list, all->command_list);
	rl_clear_history();
	exit(code);
}
