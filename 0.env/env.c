/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 14:02:17 by alandel           #+#    #+#             */
/*   Updated: 2025/09/02 15:04:27 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



int	main(char **env)
{
	t_env *env;

	env = malloc(sizeof(t_env) * 4);
	if (!env)
		return (0);

	if(!env[0])

	else
		env->env = env;
	if (path)
		init_env(env);
	else
		init_local_env(env);
}