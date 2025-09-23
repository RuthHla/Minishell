/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:06:38 by adenny            #+#    #+#             */
/*   Updated: 2025/09/17 13:07:26 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static const char	*cd_resolve_target(t_element *head, t_shell *sh)
{
	const char	*home;
	t_element	*e;

	e = head;
	if (e && e->kind == ARG)
		e = e->next;
	while (e && (e->kind != ARG || !e->u_.arg || !e->u_.arg->str
			|| !*e->u_.arg->str))
		e = e->next;
	if (!e)
	{
		home = find_variable_in_env(sh->env, "HOME");
		if (!home)
		{
			write(STDERR_FILENO, "minishell: cd: HOME not set\n",
				sizeof("minishell: cd: HOME not set\n") - 1);
			return (NULL);
		}
		return (home);
	}
	return (e->u_.arg->str);
}

static int	cd_update_env(t_shell *sh, const char *oldpwd, const char *newpwd)
{
	if (!setenv_in_vec_cd(&sh->env, "OLDPWD", oldpwd))
		return (0);
	if (!setenv_in_vec_cd(&sh->env, "PWD", newpwd))
		return (0);
	return (1);
}

static int	check_how_many_arg(t_element *current)
{
	int	i;

	i = 0;
	while (current && current->kind == ARG)
	{
		i++;
		current = current->next;
	}
	if (i > 2)
		return (0);
	return (1);
}

int	builtin_cd(t_command *cmd, t_shell *sh)
{
	const char	*target;
	char		oldpwd[4096];
	char		newpwd[4096];
	t_element	*current;

	current = cmd->element;
	if (!check_how_many_arg(current))
		return (1);
	target = cd_resolve_target(cmd->element, sh);
	if (!target)
		return (1);
	if (!getcwd(oldpwd, sizeof(oldpwd)))
		oldpwd[0] = '\0';
	if (chdir(target) != 0)
		return (1);
	if (!getcwd(newpwd, sizeof(newpwd)))
		return (0);
	if (!cd_update_env(sh, oldpwd, newpwd))
		return (0);
	return (0);
}
