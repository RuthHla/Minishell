/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 09:06:51 by adenny            #+#    #+#             */
/*   Updated: 2025/09/17 17:17:55 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_heredoc_loop(int pfd, const char *delim)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strlen(line) == ft_strlen(delim) && ft_strncmp(line, delim,
				ft_strlen(delim)) == 0)
		{
			free(line);
			break ;
		}
		write(pfd, line, ft_strlen(line));
		write(pfd, "\n", 1);
		free(line);
	}
}

int	create_heredoc_fd(const char *delim)
{
	int	pfd[2];

	if (pipe(pfd) < 0)
	{
		perror("pipe");
		return (-1);
	}
	handle_heredoc_loop(pfd[1], delim);
	close(pfd[1]);
	return (pfd[0]);
}
