/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_norme.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adenny <adenny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 09:27:55 by adenny            #+#    #+#             */
/*   Updated: 2025/09/16 09:39:18 by adenny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	apply_redirs(const t_ios *ios)
{
	if (ios->in_fd >= 0)
	{
		if (dup2(ios->in_fd, STDIN_FILENO) < 0)
		{
			perror("dup2");
			return (0);
		}
	}
	if (ios->out_fd >= 0)
	{
		if (dup2(ios->out_fd, STDOUT_FILENO) < 0)
		{
			perror("dup2");
			return (0);
		}
	}
	return (1);
}

void	close_redirs(t_ios *ios)
{
	if (ios->in_fd >= 0)
	{
		close(ios->in_fd);
		ios->in_fd = -1;
	}
	if (ios->out_fd >= 0)
	{
		close(ios->out_fd);
		ios->out_fd = -1;
	}
}
