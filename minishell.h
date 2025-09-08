#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>             
# include <dirent.h>
# include <errno.h>
# include <readline/history.h>   
# include <readline/readline.h>
# include <signal.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>
# include "libft.h"
# include "0.env/env.h"
# include "4.expander/expander.h"

extern int 		g_signal_received;

typedef enum e_type
{
	LITERAL,
	PIPE,
	REDIR_IN,
	HEREDOC,
	REDIR_OUT,
	APPEND,
	DOLLAR,
	SPECIAL_VARIABLE,
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
	struct s_token		*next;
}						t_token;

typedef struct s_redir
{
	t_type				type;
	t_type				target_type;
	char				*target;
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

typedef enum e_element_kind
{
	ARG,
	REDIR,
}						t_element_kind;

typedef struct s_element
{
	t_element_kind		kind;
	union
	{
		t_redir			*redirs;
		t_arg			*arg;
	} u_;
	struct s_element	*next;
}						t_element;

typedef struct s_command
{
	t_type_cmd			cmd;
	t_element			*element;
	size_t				nb_args;
	int					has_pipe_out;
	struct s_command	*next;
	struct s_command	*previous;
}						t_command;

typedef struct s_shell
{
	char	**env;
	int		last_exit; //pour $?
	int 	shlvl;
}			t_shell;

typedef struct s_ios
{
    int in_fd;   // fd ouvert pour l'entrée (stdin redirigé)
    int out_fd;  // fd ouvert pour la sortie (stdout redirigé)
}   t_ios;

// Lexer / utils
int						ft_isspace(char c);
t_ctx					get_ctx_type(char c);
t_type					get_character_type(char c);
int						handle_quote_context(char c, t_ctx *current_context);
void					free_character_list(t_character *head);

// Lexer / char
t_type					get_character_type(char c);
t_character				*build_char_list(char *line);

// Tokenizer / dollar
t_token					*create_token_from_chars(t_character *chars,
							int word_id);

// Token functions
char					*build_token_string(t_character *chars, int len);
t_type					get_operator_token_type(t_character *chars);
t_token					*convert_to_tokens(t_character *chars);

// tokenizer -> Utils.c
void					free_token_list(t_token *head);
int						valid_variable_char(char c);
int						is_operator_char(char c);
int						same_word(t_character *a, t_character *b);
t_token					*new_token(t_type type, size_t len);
void					append_token(t_token **head, t_token **tail,
							t_token *node);

// tokenizer / tokenize.c
int						create_normal_token(t_token **h, t_token **t,
							t_character **p);
int						create_operator_token(t_token **h, t_token **t,
							t_character **p);
t_token					*build_token_list(t_character *char_list);

// init.c
t_command				*create_new_command(void);
t_command				*init_struct_globale(t_token *token_list, char **line);

// Parser / parse_token.c
int						parse_token(t_token *token_list);

// utils.c
t_element				*lst_last_node(t_element *head);
int						is_redir(t_type type);
int						is_operator(t_type type);
int						is_command(t_type_cmd type);
t_type_cmd				identify_builtin(const char *str);
t_arg					*create_arg(t_type type, const char *str);
t_redir					*create_redir(t_type type, const char *target,
							t_type target_type);
t_element				*create_element_arg(t_type type, const char *str);
t_element				*create_element_redir(t_type type, const char *target,
							t_type target_type);
void					add_element_to_command(t_command *cmd,
							t_element *element);

// free.c
void					cleanup(t_command *cmd);
void					cleanall_exit(t_command *cmd, t_token *token_list,
							char **line);

// cmd.c
int						save_all(t_command *cmd, t_token *token_list,
							char **line);
int						add_cmd(t_command *cmd, char *str);
int						add_argument(t_command *cmd, t_type type,
							const char *str);
// env.c 
void    init_shell_shlvl(t_shell *shell, char **envp);

// signals.c
void    signal_handler(int sig);
void    check_signals(void);
void    reset_signals_for_child(void);
void    setup_signals(void);

// expander.c
void					expander(t_command **cmd_list, t_shell *shell);

// exec
int   run_pipeline(t_command *cmd_list, t_shell *sh);

// redic
void close_redirs(t_ios *ios);
int apply_redirs(const t_ios *ios);
int collect_redirs_fds(t_element *elem, t_ios *ios);

// heredoc
int create_heredoc_fd(const char *delim);

#endif