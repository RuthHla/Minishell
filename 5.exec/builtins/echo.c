/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 09:47:21 by adenny            #+#    #+#             */
/*   Updated: 2025/09/17 13:04:45 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_echo_n(const char *s)
{
	int	i;

	if (!s || s[0] != '-')
		return (0);
	i = 1;
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}

static void	print_args(t_element *e)
{
	int	first;

	first = 1;
	while (e)
	{
		if (e->kind == ARG && e->u_.arg && e->u_.arg->str)
		{
			if (!first)
				write(STDOUT_FILENO, " ", 1);
			write(STDOUT_FILENO, e->u_.arg->str,
				ft_strlen(e->u_.arg->str));
			first = 0;
		}
		e = e->next;
	}
}

int	builtin_echo(t_command *cmd, t_shell *sh)
{
	int			no_newline;
	t_element	*e;

	(void)sh;
	no_newline = 0;
	e = cmd->element;
	if (e && e->kind == ARG)
		e = e->next;
	while (e && e->kind == ARG && e->u_.arg
		&& is_echo_n(e->u_.arg->str))
	{
		no_newline = 1;
		e = e->next;
	}
	print_args(e);
	if (!no_newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
