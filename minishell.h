/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alandel <alandel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:24:41 by alandel           #+#    #+#             */
/*   Updated: 2025/09/25 10:55:13 by alandel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
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
# include "1.lexer/lexer.h"
# include "2.tokenizer/tokenize.h"
# include "4.expander/expander.h"

extern int				g_signal_received;

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
	char				**env;
	int					last_exit;
	int					shlvl;
}						t_shell;

typedef struct s_all
{
	t_character			*char_list;
	t_token				*token_list;
	t_command			*command_list;
}						t_all;

typedef struct s_ios
{
	int					in_fd;
	int					out_fd;
}						t_ios;

typedef struct s_launch_ctx
{
	pid_t				*pids;
	int					*out_n;
	t_shell				*sh;
}						t_launch_ctx;

typedef struct s_pipeinfo
{
	int					pfd[2];
	int					need_pipe;
	int					out_wr;
}						t_pipeinfo;

// 1.Lexer / utils
int						ft_isspace(char c);
int						is_operator_char(char c);
t_ctx					get_ctx_type(char c);
t_type					get_character_type(char c);
int						handle_quote_context(char c, t_ctx *current_context);

// 1.Lexer / char
t_type					get_character_type(char c);
t_character				*build_char_list(char *line);

// 2.tokenizer / tokenize.c
t_token					*build_token_list(t_character *char_list);

// 3.Parser / add.c
int						add_cmd(t_command *cmd, char *str);
int						add_argument(t_command *cmd, t_type type,
							const char *str);
int						add_redir(t_token **token_list, t_command **current);
int						handle_pipe(t_token **token_list, t_command **current);
int						handle_cmd_or_arg(t_token **token_list,
							t_command **current);

// 3.Parser / cmd.c
int						save_all(t_command *cmd, t_token *token_list);
t_command				*create_new_command(void);
t_command				*init_struct_globale(t_token *token_list);

// 3.Parser / init.c
t_arg					*create_arg(t_type type, const char *str);
t_redir					*create_redir(t_type type, const char *target,
							t_type target_type);
t_element				*create_element_arg(t_type type, const char *str);
t_element				*create_element_redir(t_type type, const char *target,
							t_type target_type);
void					add_element_to_command(t_command *cmd,
							t_element *element);

// 3.Parser / parse_token.c
int						parse_token(t_token *token_list);

// 3.Parser / utils.c
t_element				*lst_last_node(t_element *head);
int						is_redir(t_type type);
int						is_operator(t_type type);
int						is_command(t_type_cmd type);
t_type_cmd				identify_builtin(const char *str);

// 0.env / env.c
void					init_shell_shlvl(t_shell *shell, char **envp);

// 0.env / signals.c
void					signal_handler(int sig);
int						check_signals(void);
void					reset_signals_for_child(void);
void					setup_signals(void);

// expander.c
void					expander(t_command **cmd_list, t_shell *shell);

// norme
int						apply_redir(t_ios *ios, t_redir *r);
int						is_var_start(int c);
int						is_var_continue(int c);
int						is_valid_ident(const char *s);
void					remove_env_var(char ***penv, const char *name);
size_t					count_args(t_element *e);
int						is_numeric_word(const char *s);
char					to_exit_u8(const char *s);
int						is_valid_ident_export(const char *s);
char					*dup_n(const char *s, size_t n);
char					*make_env_kv(const char *name, const char *value);
int						setenv_in_vec(char ***penv, const char *name,
							const char *value);
void					fill_argv(t_element *e, char **argv);
char					**build_export_argv(t_element *e);
int						print_all_exports(t_shell *sh);
int						process_export_args(char **argv, int idx, t_shell *sh);
int						setenv_in_vec_cd(char ***penv, const char *name,
							const char *value);
int						exit_status_from_errno(int e);
int						is_directory(const char *p);
void					print_exec_error(const char *path, const char *argv0,
							int e);
char					*join_path(const char *dir, const char *cmd);
char					*resolve_in_path(const char *cmd, char **env);
// void					free_argv(char **argv);
int						is_last_cmd(t_command *cmd);
int						is_simple_builtin_type(t_type_cmd c);
int						exec_with_path(char **argv, t_shell *sh);
void					print2_err(const char *a, const char *b);
void					putstr_err(const char *s);
size_t					count_args_nonempty(const t_command *cmd);
void					free_argv_dup(char **argv);
int						child_prepare_fds(t_command *cmd, int prev_rd,
							int out_wr);
pid_t					spawn_one(t_all *all, t_command *cmd, int prev_rd,
							int out_wr, t_shell *sh);
int						wait_all(pid_t *pids, int n);
int						run_single_builtin(t_command *cmd, t_shell *sh,
							t_all *all);
void					restore_stdio_and_close(int saved_in, int saved_out);
int						apply_redirs_for_single(t_command *cmd, int saved_in,
							int saved_out);
int						save_stdio(int *saved_in, int *saved_out);
void					advance_pipe_state(int *prev_rd, t_pipeinfo *pi);
void					cleanup_on_fail(int *prev_rd, t_pipeinfo *pi);

// exec
int						run_pipeline(t_all *all, t_command *cmd_list,
							t_shell *sh);
int						exec_builtin(t_command *cmd, t_shell *sh, t_all *all);

// redic
void					close_redirs(t_ios *ios);
int						apply_redirs(const t_ios *ios);
int						collect_redirs_fds(t_element *elem, t_ios *ios);
int						open_in(const char *path);
int						open_out_trunc(const char *path);
int						open_out_append(const char *path);

// heredoc
int						create_heredoc_fd(const char *delim);

// builtin
int						builtin_echo(t_command *cmd, t_shell *sh);
int						builtin_pwd(t_command *cmd, t_shell *sh);
int						builtin_cd(t_command *cmd, t_shell *sh);
int						builtin_export(t_command *cmd, t_shell *sh);
int						builtin_unset(t_command *cmd, t_shell *sh);
int						builtin_env(t_command *cmd, t_shell *sh);
int						builtin_exit(t_command *cmd, t_shell *sh, t_all *all);

// Helper de détection
void					print_export_line(const char *entry);
void					print_export_error(const char *s);
int						handle_export_arg(const char *arg, t_shell *sh);
int						is_builtin_cmd(t_type_cmd cmd);

// 6.free/ free.c
t_all					*get_all(void);
void					free_env(t_shell *shell);
void					free_character_list(t_character *head);
void					free_token_list(t_token *head);

// 6.free/ free2.c
void					cleanup(t_command *cmd);
void					cleanall(t_character *char_list, t_token *token_list,
							t_command *cmd);

// free/ 6.error
void					ft_putstr_fd(char *s, int fd);
void					print_error(char *msg);
void					print_syntax_error(char *token);

#endif