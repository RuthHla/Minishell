#include "../minishell.h"

// gestion des dollars classiques OK
// manque special ?! et $ et $1 != $a4 attention ausi a echo $"ca va" (il faut verifier que nai pas de quote qd jenregistre en variable)
// traiter les redirections
// si la varaiable commence par un caractere stopant lexpansion alors afficher brute avec le dollar "echo $/",
// sinon rien afficher (car pas trouver dans les variables denvironnements) puis ce qui suit (type LITERAL)

static void	merge_node(t_token **head, t_token **tail, t_token *new_node)
{
	if (*head == NULL)
	{
		*head = new_node;
		*tail = new_node;
	}
	else
	{
		(*tail)->next = new_node;
		*tail = new_node;
	}
}

static t_token	*init_node(t_token **head, t_token **tail,
		t_character *char_list, int flag)
{
	int		len;
	t_token	*new_node;

	len = 0;
	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->str = NULL;
	new_node->next = NULL;
	if (flag == 1)
		new_node->type = DOLLAR;
	else if (flag == 2)
		new_node->type = SPECIAL_VARIABLE;
	else
		new_node->type = char_list->type;
	len = get_token_len(char_list, char_list->type, char_list->word_id,
			flag);
	new_node->str = malloc(sizeof(char) * (len + 1));
	if (!new_node->str)
	{
		free(new_node);
		return (NULL);
	}
	merge_node(head, tail, new_node);
	return (new_node);
}

static void	fill_str(t_token **tail, t_character **char_list,
		t_type current_type, int current_word, int flag)
{
	int			i;
	t_character	*c_list;

	c_list = (*char_list);
	i = 0;
	if (flag == 1)
	{
		while (c_list && valid_variable_char(c_list->c))
		{
			(*tail)->str[i++] = (c_list)->c;
			(c_list) = (c_list)->next;
		}
		(*tail)->str[i] = '\0';
		(*char_list) = c_list;
		return;
	}
	else if (flag == 2)
	{
		(*tail)->str[i++] = '$';
		(*tail)->str[i++] = c_list->next->c;
		(*tail)->str[i] = '\0';
		*char_list = c_list->next->next;
		return;
	}
	
	while(c_list && c_list->type == current_type && c_list->word_id == current_word)
	{
		(*tail)->str[i++] = c_list->c;
		c_list = c_list->next;
	}
	(*tail)->str[i] = '\0';
	*char_list = c_list;
}

t_token	*build_token_list(t_character *char_list)
{
	t_token	*head;
	t_token	*tail;
	int		current_word;
	t_type	current_type;
	int		flag;

	head = NULL;
	tail = NULL;
	while (char_list)
	{
		flag = 0;
		(void)handle_dollar(&char_list, &flag);
		if (!init_node(&head, &tail, char_list, flag))
		{
			free_token_list(head);
			return (NULL);
		}
		current_word = char_list->word_id;
		current_type = char_list->type;
		fill_str(&tail, &char_list, current_type, current_word, flag);
	}
	return (head);
}

int	handle_dollar(t_character **char_list, int *flag)
{
	t_character	*next;
	t_character	*c;

	if (!char_list || !*char_list)
		return (0);
	c = *char_list;
	if (c->c != '$' || c->context == S_QUOTE)
		return (0);
	next = c->next;
	if (!next || (c->word_id != next->word_id))
		return (0);
	if (next && next->context == NONE && special_variable(next->c))
	{
		*flag = 2;
		*char_list = c;
		return (1);
	}
	if (next && valid_variable_char(next->c))
	{
		*flag = 1;
		*char_list = next;
		return (1);
	}
	return (0);
}
