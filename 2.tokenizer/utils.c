#include "../minishell.h"

int	valid_variable_char(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0'
			&& c <= '9') || c == '_')
		return (1);
	return (0);
}

void	free_token_list(t_token *head) // verfier si assez securise
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->str);
		free(head);
		head = tmp;
	}
}

int	get_token_len(t_character *char_list, t_type current_type, int current_word, int flag)
{
	int			len;
	t_character	*list;

	len = 0;
	list = char_list;
	if(flag == 0)
	{
		while (list && list->word_id == current_word && list->type == current_type)
		{
			len++;
			list = list->next;
		}
	}
	else if (flag == 1) //cas classique de variable valide
	{
		while (list && valid_variable_char(list->c))
		{
			len++;
			list = list->next;
		}
	}

	else if (flag == 2) // speicale variable la len est a 2
		len = 2;

	return (len);
}

int special_variable(char c)
{
	if ((c >= '0' && c <= '9') || (c == '?'))
		return 1;
	return 0;
}