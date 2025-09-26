/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 09:06:51 by adenny            #+#    #+#             */
/*   Updated: 2025/09/26 13:18:34 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	handle_heredoc_loop(int fd, char *delim)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("warning: here-document delimited ", 2);
			ft_putstr_fd("by end-of-file (wanted `", 2);
			ft_putstr_fd(delim, 2);
			ft_putendl_fd("')", 2);
			break ;
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

static void	heredoc_child_process(int *pfd, char *delim)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	handle_heredoc_loop(pfd[1], delim);
	close(pfd[1]);
	_exit(0);
}

static int	handle_heredoc_status(int status, int fd, t_shell *sh)
{
	if (WIFSIGNALED(status))
	{
		ft_putendl_fd("", 2);
		sh->last_exit = 128 + WTERMSIG(status);
		close(fd);
		return (-1);
	}
	else if (WIFEXITED(status))
		sh->last_exit = WEXITSTATUS(status);
	return (fd);
}

int	create_heredoc_fd(char *delim, t_shell *sh)
{
	int		pfd[2];
	pid_t	pid;
	int		status;

	pid = fork();
	if (pipe(pfd) < 0 || pid < 0)
		return (-1);
	if (pid == 0)
		heredoc_child_process(pfd, delim);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	close(pfd[1]);
	if (waitpid(pid, &status, 0) == -1)
		status = 1;
	setup_signals();
	return (handle_heredoc_status(status, pfd[0], sh));
}
