/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_norme6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:19:03 by adenny            #+#    #+#             */
/*   Updated: 2025/09/16 13:20:10 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	save_stdio(int *saved_in, int *saved_out)
{
	*saved_in = dup(STDIN_FILENO);
	*saved_out = dup(STDOUT_FILENO);
	if (*saved_in < 0 || *saved_out < 0)
	{
		perror("dup");
		if (*saved_in >= 0)
			close(*saved_in);
		if (*saved_out >= 0)
			close(*saved_out);
		return (0);
	}
	return (1);
}

int	apply_redirs_for_single(t_command *cmd, int saved_in, int saved_out)
{
	t_ios	ios;

	if (!collect_redirs_fds(cmd->element, &ios))
	{
		if (saved_in >= 0)
			close(saved_in);
		if (saved_out >= 0)
			close(saved_out);
		return (0);
	}
	if (!apply_redirs(&ios))
	{
		close_redirs(&ios);
		if (saved_in >= 0)
			close(saved_in);
		if (saved_out >= 0)
			close(saved_out);
		return (0);
	}
	close_redirs(&ios);
	return (1);
}

void	restore_stdio_and_close(int saved_in, int saved_out)
{
	if (dup2(saved_in, STDIN_FILENO) < 0)
		perror("dup2");
	if (dup2(saved_out, STDOUT_FILENO) < 0)
		perror("dup2");
	if (saved_in >= 0)
		close(saved_in);
	if (saved_out >= 0)
		close(saved_out);
}

// int	run_single_builtin(t_command *cmd, t_shell *sh, t_all *all)
// {
// 	int	saved_in;
// 	int	saved_out;
// 	int	code;

// 	if (!save_stdio(&saved_in, &saved_out))
// 		return (1);
// 	if (!apply_redirs_for_single(cmd, saved_in, saved_out))
// 		return (1);
// 	code = exec_builtin(cmd, sh, all);
// 	restore_stdio_and_close(saved_in, saved_out);
// 	sh->last_exit = code;
// 	return (code);
// }

int	run_single_builtin(t_command *cmd, t_shell *sh, t_all *all)
{
	int	saved_in;
	int	saved_out;
	int	code;

	if (!save_stdio(&saved_in, &saved_out))
		return (1);
	if (!apply_redirs_for_single(cmd, saved_in, saved_out))
		return (1);
	if (cmd->cmd == T_EXIT)
	{
		restore_stdio_and_close(saved_in, saved_out);
		exec_builtin(cmd, sh, all);
	}
	code = exec_builtin(cmd, sh, all);
	restore_stdio_and_close(saved_in, saved_out);
	sh->last_exit = code;
	return (code);
}
