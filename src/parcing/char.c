#include "../../minishell.h"

t_type get_character_type(char c)
{
	if ((c >= 'a' && c <= 'z') || 
	    (c >= 'A' && c <= 'Z') || 
	    (c >= '0' && c <= '9')||
		c == '.' || c == '/' || c == '_')
		return LITERAL;
	else if (c == '|')
		return PIPE;
	else if (c == '&')
		return AMPERSAND;
	else if (c == '<')
		return REDIR_IN;
	else if (c == '>')
		return REDIR_OUT;
	else if (c == '$')
		return DOLLAR;
	else if (c == 39)
		return QUOTE;
	else if (c == 34)
		return DOUBLE_QUOTE;
	return UNKNOWN;
}

void	free_character_list(t_character *head)
{
	t_character *tmp;

	while (head)
	{
		tmp = head->next;
		free(head);
		head = tmp;
	}
}

int malloc_structure_character(t_character **t, int n)
{
	t_character *head = NULL;
	t_character *curr = NULL;

	while (n > 0)
	{
		t_character *new = malloc(sizeof(t_character));
		if (!new)
		{
			free_character_list(head);
			return 0;
		}
		new->c = 0;
		new->type = UNKNOWN;
		new->next = NULL;
		if (!head)
			head = new;
		else
			curr->next = new;

		curr = new;
		n--;
	}
	*t = head;
	return 1;
}