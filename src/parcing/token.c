#include "../../minishell.h"

void free_token_list(t_token *head)
{
	t_token *tmp;
	while (head)
	{
		tmp = head->next;
		free(head->str);
		free(head);
		head = tmp;
	}
}

int get_word_len(t_character *chars, int word)
{
	int len = 0;
	while (chars && chars->word == word)
	{
		len++;
		chars = chars->next;
	}
	return len;
}

char *build_token_string(t_character *chars, int len)
{
	char *str = malloc(len + 1);
	int i = 0;
	if (!str)
		return NULL;
	while (i < len && chars)
	{
		str[i++] = chars->c;
		chars = chars->next;
	}
	str[i] = '\0';
	return str;
}

t_type get_operator_token_type(t_character *chars)
{
	if (chars->c == '<') return HEREDOC;
	if (chars->c == '>') return APPEND;
	if (chars->c == '|') return OR;
	if (chars->c == '&') return AND;
	return chars->type;
}
