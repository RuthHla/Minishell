/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 14:16:04 by alandel           #+#    #+#             */
/*   Updated: 2025/09/16 11:40:55 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZE_H
# define TOKENIZE_H
# include "../minishell.h"

typedef enum e_type			t_type;
typedef struct s_character	t_character;
typedef struct s_token		t_token;

void						detect_operator_type(t_character *c, t_character *n,
								t_type *type, size_t *len);
int							is_expandable_dollar(t_character *dollar_char);
int							word_has_special_variable(t_character *word_start);
int							word_has_expandable_dollar(t_character *word_start);
size_t						get_word_length(t_character *start);

int							is_operator_type(t_type type);
int							valid_variable_char(char c);
int							same_word(t_character *a, t_character *b);
t_token						*new_token(t_type type, size_t len);
void						append_token(t_token **head, t_token **tail,
								t_token *node);

#endif