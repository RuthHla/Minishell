NAME_LEX := test_lexer
NAME_TOK := test_tokenizer
NAME_PAR := test_parser
NAME_EXP := test_expander
NAME_EXEC := minishell

CC := cc

# SUE LINUX
 CFLAGS := -Wall -Wextra -Werror -MMD -MP -I. -ILibft
 LDFLAGS := -lreadline

# SUR MAC 
# BREW_PREFIX := $(shell brew --prefix 2>/dev/null || echo /usr/local)
# CFLAGS := -Wall -Wextra -Werror -MMD -MP -I. -ILibft -I$(BREW_PREFIX)/opt/readline/include
# LDFLAGS := -L$(BREW_PREFIX)/opt/readline/lib -lreadline

# Libft
LIBFT_DIR := Libft
LIBFT := $(LIBFT_DIR)/libft.a

SRC_LEXER := 1.lexer/char.c 1.lexer/utils.c 1.lexer/init.c 6.free/free.c 6.free/free2.c 6.free/error.c
SRC_TOKENIZER := 2.tokenizer/tokenize.c 2.tokenizer/utils.c 2.tokenizer/local_utils.c
SRC_PARSER := 3.parser/init.c 3.parser/utils.c 3.parser/parse_tokens.c 3.parser/cmd.c 3.parser/add.c
SRC_EXPANDER := 0.env/env.c  0.env/utils.c 0.env/signals.c 0.env/shlvl.c 4.expander/expander.c 4.expander/utils0.c 4.expander/utils1.c 4.expander/utils2.c 4.expander/utils3.c
SRC_EXEC := 5.exec/exec.c 5.exec/heredoc.c 5.exec/redir.c 5.exec/builtins/echo.c 5.exec/builtins/cd.c 5.exec/builtins/pwd.c 5.exec/builtins/export.c 5.exec/builtins/unset.c 5.exec/builtins/env.c 5.exec/builtins/exit.c 5.exec/norme/redir_norme.c 5.exec/norme/redir_norme2.c 5.exec/norme/unset_norme.c 5.exec/norme/exit_norme.c 5.exec/norme/export_norme.c 5.exec/norme/export_norme2.c 5.exec/norme/cd_norme.c 5.exec/norme/exec_norme.c 5.exec/norme/exec_norme2.c 5.exec/norme/exec_norme3.c 5.exec/norme/exec_norme4.c 5.exec/norme/exec_norme5.c 5.exec/norme/exec_norme6.c 5.exec/norme/exec_norme7.c
SRC_MINISHELL := minishell.c $(SRC_LEXER) $(SRC_TOKENIZER) $(SRC_PARSER)
SRC_TEST_LEX := tests/lexer.c $(SRC_LEXER)
SRC_TEST_TOK := tests/tokenizer.c $(SRC_LEXER) $(SRC_TOKENIZER)
SRC_TEST_PAR := tests/parser.c $(SRC_LEXER) $(SRC_TOKENIZER) $(SRC_PARSER)
SRC_TEST_EXP := tests/test_expander.c $(SRC_LEXER) $(SRC_TOKENIZER) $(SRC_PARSER) $(SRC_EXPANDER)
SRC_TEST_EXEC := main.c $(SRC_LEXER) $(SRC_TOKENIZER) $(SRC_PARSER) $(SRC_EXPANDER) $(SRC_EXEC)

OBJDIR := obj
OBJDIR_MINI := $(OBJDIR)/minishell
OBJDIR_TLEX := $(OBJDIR)/test_lexer
OBJDIR_TTOK := $(OBJDIR)/test_tokenizer
OBJDIR_TPAR := $(OBJDIR)/test_parser
OBJDIR_TEXP := $(OBJDIR)/test_expander
OBJDIR_TEXEC := $(OBJDIR)/test_exec

OBJS_MINI := $(SRC_MINISHELL:%.c=$(OBJDIR_MINI)/%.o)
OBJS_TLEX := $(SRC_TEST_LEX:%.c=$(OBJDIR_TLEX)/%.o)
OBJS_TTOK := $(SRC_TEST_TOK:%.c=$(OBJDIR_TTOK)/%.o)
OBJS_TPAR := $(SRC_TEST_PAR:%.c=$(OBJDIR_TPAR)/%.o)
OBJS_TEXP := $(SRC_TEST_EXP:%.c=$(OBJDIR_TEXP)/%.o)
OBJS_TEXEC := $(SRC_TEST_EXEC:%.c=$(OBJDIR_TEXEC)/%.o)

DEPS := $(OBJS_MINI:.o=.d) $(OBJS_TLEX:.o=.d) $(OBJS_TTOK:.o=.d) $(OBJS_TPAR:.o=.d) $(OBJS_TEXP:.o=.d) $(OBJS_TEXEC:.o=.d)

.PHONY: all clean fclean re test-lexer test-tokenizer test-parser test-expander test-exec

all: $(NAME_EXEC)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS_MINI)
	$(CC) $(CFLAGS) $(OBJS_MINI) $(LIBFT) -o $@ $(LDFLAGS)

test-lexer: $(NAME_LEX)
$(NAME_LEX): $(LIBFT) $(OBJS_TLEX)
	$(CC) $(CFLAGS) $(OBJS_TLEX) $(LIBFT) -o $@ $(LDFLAGS)

test-tokenizer: $(NAME_TOK)
$(NAME_TOK): $(LIBFT) $(OBJS_TTOK)
	$(CC) $(CFLAGS) $(OBJS_TTOK) $(LIBFT) -o $@ $(LDFLAGS)

test-parser: $(NAME_PAR)
$(NAME_PAR): $(LIBFT) $(OBJS_TPAR)
	$(CC) $(CFLAGS) $(OBJS_TPAR) $(LIBFT) -o $@ $(LDFLAGS)

test-expander: $(NAME_EXP)
$(NAME_EXP): $(LIBFT) $(OBJS_TEXP)
	$(CC) $(CFLAGS) $(OBJS_TEXP) $(LIBFT) -o $@ $(LDFLAGS)

test-exec: $(NAME_EXEC)
$(NAME_EXEC): $(LIBFT) $(OBJS_TEXEC)
	$(CC) $(CFLAGS) $(OBJS_TEXEC) $(LIBFT) -o $@ $(LDFLAGS)

$(OBJDIR_MINI)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR_TLEX)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR_TTOK)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR_TPAR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR_TEXP)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR_TEXEC)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) $(NAME_LEX) $(NAME_TOK) $(NAME_PAR) $(NAME_EXP) $(NAME_EXEC)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

-include $(DEPS)
