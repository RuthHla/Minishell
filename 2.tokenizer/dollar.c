#include "../minishell.h"

// IMPORTANT -> securiser les fonctions

int	create_variable_token(t_token **head, t_token **tail,
		t_character **char_list)
{
	t_character	*current_char;
	t_character	*c_next;
	size_t		len;
	size_t		j;
	t_token		*tok;

	current_char = *char_list;
	c_next = current_char->next;
	len = 0;
	while (c_next && same_word(current_char, c_next)
		&& valid_variable_char(c_next->c))
	{
		len++;
		c_next = c_next->next;
	}
	if (len == 0)
		return (1);
	c_next = current_char->next;
	{
		tok = new_token(DOLLAR, len);
		if (!tok)
			return (0);
		j = 0;
		while (j < len && c_next)
		{
			tok->str[j++] = c_next->c;
			c_next = c_next->next;
		}
		tok->str[len] = '\0';
		append_token(head, tail, tok);
		*char_list = c_next;
	}
	return (1);
}

int	create_special_variable_token(t_token **head, t_token **tail,
		t_character **char_list)
{
	t_character	*current_char;
	t_character	*next;
	t_token		*tok;

	current_char = *char_list;
	next = NULL;
	tok = NULL;
	if (current_char->next)
		next = current_char->next;
	if (!next)
		return (1);
	tok = new_token(SPECIAL_VARIABLE, 1);
	if (!tok)
		return (0);
	tok->str[0] = next->c;
	tok->str[1] = '\0';
	append_token(head, tail, tok);
	*char_list = next->next;
	return (1);
}

int	create_dollar_literal(t_token **head, t_token **tail,
		t_character **char_list)
{
	t_character	*first_pos;
	t_character	*i_list;
	size_t		len;
	size_t		j;
	t_token		*tok;

	first_pos = *char_list;
	i_list = first_pos;
	len = 0;
	while (i_list && same_word(first_pos, i_list))
	{
		len++;
		i_list = i_list->next;
	}
	i_list = first_pos;
	tok = new_token(LITERAL, len);
	if (!tok)
		return (0);
	j = 0;
	while (j < len && i_list)
	{
		tok->str[j++] = i_list->c;
		i_list = i_list->next;
	}
	tok->str[len] = '\0';
	append_token(head, tail, tok);
	*char_list = i_list;
	return (1);
}

int	create_single_dollar_literal(t_token **head, t_token **tail,
		t_character **char_list)
{
	t_character	*current_char;
	t_token		*tok;

	current_char = *char_list;
	tok = new_token(LITERAL, 1);
	if (!tok)
		return (0);
	tok->str[0] = '$';
	tok->str[1] = '\0';
	append_token(head, tail, tok);
	*char_list = current_char->next;
	return (1);
}

int	create_dollar_quoted_token(t_token **head, t_token **tail,
		t_character **char_list)
{
	t_character	*c;
	t_character	*first_pos;
	size_t		len;
	size_t		j;
	t_token		*tok;

	c = (*char_list)->next;
	first_pos = (*char_list)->next;
	len = 0;
	while (c && same_word(first_pos, c))
	{
		len++;
		c = c->next;
	}
	if (len == 0)
		return (1);
	c = (*char_list)->next;
	tok = new_token(LITERAL, len);
	if (!tok)
		return (0);
	j = 0;
	while (j < len && c)
	{
		tok->str[j++] = c->c;
		c = c->next;
	}
	tok->str[len] = '\0';
	append_token(head, tail, tok);
	*char_list = c;
	return (1);
}
