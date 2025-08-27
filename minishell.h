#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

typedef enum e_type
{
	LITERAL,
	PIPE,
	AMPERSAND, // a gerer ?
	REDIR_IN,
	HEREDOC,
	REDIR_OUT,
	APPEND,
	DOLLAR,
	UNKNOWN,
}						t_type;

typedef enum s_ctx
{
	NONE,
	D_QUOTE,
	S_QUOTE,
}						t_ctx;

typedef struct s_character
{
	char				c;
	int					word_id;
	t_type				type;
	t_ctx				context;
	struct s_character	*prev;
	struct s_character	*next;
}						t_character;

typedef struct s_token
{
	char				*str;
	t_type				type;
	// struct s_token		*prev;
	struct s_token		*next;
}						t_token;

typedef struct s_redir
{
	t_type				type;
	char				*target;
	int					error;
	struct s_redir		*next;
}						t_redir;

typedef enum e_cmd
{
	CMD_NONE,
	T_ECHO,
	T_CD,
	T_PWD,
	T_EXPORT,
	T_UNSET,
	T_ENV,
	T_EXIT,
	NOT_BUILTIN,
}						t_type_cmd;

typedef struct s_arg
{
	char				*str;
	t_type				type;
}						t_arg;

typedef struct s_command
{
	t_type_cmd			cmd;
	t_arg				*args;
	// tableau de t_arg (execve attend un tableau char *argv[] terminé par NULL)
	size_t				nb_args;
	t_redir				*redirs;
	// liste chainee de redirection (noeud contenant error si pas de target,
	// la target est enregistree dans le noeud)
	int					has_pipe_out;
	struct s_command	*next;
	struct s_command	*previous;
}						t_command;

typedef struct s_build_state
{
	int					in_word;
	int					word;
	char				last_op;
}						t_build_state;

typedef struct s_charbuilder
{
	t_character			*head;
	t_character			*curr;
	t_build_state		state;
}						t_charbuilder;

// Utils
int						ft_isspace(char c);
t_ctx					get_ctx_type(char c);
t_type					get_character_type(char c);
int						handle_quote_context(char c, t_ctx *current_context);

// Norme functions
int						append_token(t_token **head, t_token **current,
							t_token *new_token);
t_token					*create_token_from_chars(t_character *chars,
							int word_id);
t_character				*add_char_node(t_charbuilder *b, char c, int type,
							int word);
int						handle_quote(const char **str, t_charbuilder *b);
int						handle_non_space(const char **str, t_charbuilder *b);
int						handle_char(const char **str, t_charbuilder *b);
int						handle_dollar(const char **str, t_charbuilder *b);

// Character functions
t_type					get_character_type(char c);
void					free_character_list(t_character *head);
t_character				*build_char_list(char *line);

// Token functions
void					free_token_list(t_token *head);
char					*build_token_string(t_character *chars, int len);
t_type					get_operator_token_type(t_character *chars);
t_token					*convert_to_tokens(t_character *chars);
t_token 				*build_token_list(t_character *char_list);
int 					get_token_len(t_character *char_list);

// init.c
t_command				*create_new_command(void);
t_command				*init_struct_globale(t_token *token_list, char **line);

// parse_token.c
int						parse_token(t_command *cmd, t_token *token_list,
							char **line);
// utils.c
void					*lst_last_node(void *head);
int						is_redir(t_type type);
int						is_operator(t_type type);
int						is_command(t_type_cmd type);
t_type_cmd				identify_builtin(const char *str);

// free.c
void					cleanup(t_command *cmd);
void					cleanall_exit(t_command *cmd, t_token *token_list,
							char **line);

// cmd.c
void					save_all(t_command *cmd, t_token *token_list,
							char **line);

#endif