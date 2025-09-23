/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 14:16:04 by alandel           #+#    #+#             */
/*   Updated: 2025/09/17 14:39:38 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H
# include "../minishell.h"

typedef struct s_shell	t_shell;

typedef struct s_var_pos
{
	char				*name;
	int					start;
	int					end;
}						t_var_pos;

// utils0.c
char					*get_variable_value(t_shell *shell, char *var_name);
int						calculate_expanded_length(char *original,
							t_var_pos *vars, char **values, int var_count);
void					copy_value(char *expanded, int *exp_idx, char *value);
char					*build_expanded_string(char *original, t_var_pos *vars,
							char **values, int var_count);
char					**get_all_values(t_shell *shell, t_var_pos *vars,
							int var_count);

// utils1.c
int						valid_variable_char_number(char c);
char					*find_variable_in_env(char **env, char *variable);
int						how_many_variable(char *str);
int						strlen_variable(char *str, int j);
char					**find_variable_in_str(char *str);

// utils2 de utils
char					*create_special_var(void);
char					*extract_normal_var(char *str, int j, int *new_pos);
void					*cleanup_variables(char **variable, int index);
int						process_dollar(char *str, int i, char **variable,
							int *index);
void					*cleanup_vars_pos(t_var_pos *vars, int index);

// utils3.c
void					free_var_data(t_var_pos *vars, char **values,
							int count);
int						extract_special_var_pos(t_var_pos *var, int start,
							int j);
int						extract_variable_pos(char *str, t_var_pos *var,
							int start, int j);
int						process_dollar_pos(char *str, int i, t_var_pos *var);
t_var_pos				*find_variables_with_positions(char *str, int *count);

#endif