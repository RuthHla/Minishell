/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 14:16:04 by alandel           #+#    #+#             */
/*   Updated: 2025/09/04 11:51:26 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H
# include "../minishell.h"

char	*itoa(int value, char *str, int base);
char	*find_variable_in_env(char **env, char *variable);
int		how_many_variable(char *str);
int		strlen_variable(char *str, int j);
char	**find_variable_in_str(char *str);
int	    valid_variable_char_number(char c);

#endif