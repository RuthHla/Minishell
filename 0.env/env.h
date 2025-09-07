/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 14:16:04 by alandel           #+#    #+#             */
/*   Updated: 2025/09/03 15:53:27 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H
# include "../minishell.h"

char	**init_local_env(void);
char	**copy_env(char **envp);
int     get_shlvl_value(char **envp);
char    *get_env_value_from_envp(char **envp, const char *var);
char    *get_env_value(char **env, const char *var);
int     count_env_vars(char **env);


#endif