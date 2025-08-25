#ifndef MINISHELL_H
# define MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <dirent.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <term.h>

typedef enum e_type
{
	LITERAL,
    PIPE,
    OR,
    AMPERSAND,
    AND,
    REDIR_IN,
    HEREDOC,
    REDIR_OUT,
    APPEND,
    VARIABLE,
    UNKNOWN,
}	t_type;

typedef struct s_character
{
    char			c;
    t_type			type;
    int word;
    struct s_character *next;
} t_character;

typedef struct s_token
{
    char *str;
    t_type type;
    struct s_token *next;
} t_token;

typedef struct s_build_state
{
	int		in_word;
	int		word;
	char	last_op;
}	t_build_state;

typedef struct s_charbuilder
{
	t_character		*head;
	t_character		*curr;
	t_build_state	state;
}	t_charbuilder;

// Utils
int ft_len_without_spaces(const char *str);
int	ft_isspace(char c);

// Norme functions
int	append_token(t_token **head, t_token **current, t_token *new_token);
t_token	*create_token_from_chars(t_character *chars, int word_id);
t_character	*add_char_node(t_charbuilder *b, char c, int type, int word);
int	handle_quote(const char **str, t_charbuilder *b);
int	handle_non_space(const char **str, t_charbuilder *b);
int	handle_char(const char **str, t_charbuilder *b);
int handle_dollar(const char **str, t_charbuilder *b);

// Character functions
t_type get_character_type(char c);
void	free_character_list(t_character *head);
int malloc_structure_character(t_character **t, int n);
t_character *build_token_list(const char *str);

// Token functions
void free_token_list(t_token *head);
int get_word_len(t_character *chars, int word);
char *build_token_string(t_character *chars, int len);
t_type get_operator_token_type(t_character *chars);
t_token *convert_to_tokens(t_character *chars);

#endif