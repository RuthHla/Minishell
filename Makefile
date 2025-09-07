# NAME       := minishell
# NAME_LEX   := test_lexer
# NAME_TOK   := test_tokenizer
# NAME_PAR   := test_parser
# NAME_EXP   := test_expander
# NAME_EXEC  := test_exec

# CC         := cc
# CFLAGS     := -Wall -Wextra -Werror -MMD -MP -I. -ILibft
# LDFLAGS    := -lreadline

# LIBFT_DIR := Libft
# LIBFT := $(LIBFT_DIR)/libft.a

# SRC_LEXER      := 1.lexer/char.c 1.lexer/utils.c
# SRC_TOKENIZER  := 2.tokenizer/tokenize.c 2.tokenizer/utils.c
# SRC_PARSER     := 3.parser/init.c 3.parser/free.c 3.parser/utils.c 3.parser/parse_tokens.c 3.parser/cmd.c
# SRC_EXPANDER   := 0.env/env.c 4.expander/expander.c 4.expander/utils.c
# SRC_EXEC	   := 5.exec/exec.c 5.exec/heredoc.c 5.exec/redir.c

# SRC_MINISHELL  := minishell.c $(SRC_LEXER) $(SRC_TOKENIZER) $(SRC_PARSER)
# SRC_TEST_LEX   := tests/lexer.c $(SRC_LEXER)
# SRC_TEST_TOK   := tests/tokenizer.c $(SRC_LEXER) $(SRC_TOKENIZER)
# SRC_TEST_PAR   := tests/parser.c $(SRC_LEXER) $(SRC_TOKENIZER) $(SRC_PARSER)
# SRC_TEST_EXP := tests/test_expander.c $(SRC_LEXER) $(SRC_TOKENIZER) $(SRC_PARSER) $(SRC_EXPANDER)
# SRC_TEST_EXEC := tests/exec.c  $(SRC_LEXER) $(SRC_TOKENIZER) $(SRC_PARSER) $(SRC_EXPANDER) $(SRC_EXEC)

# OBJDIR      := obj
# OBJDIR_MINI := $(OBJDIR)/minishell
# OBJDIR_TLEX := $(OBJDIR)/test_lexer
# OBJDIR_TTOK := $(OBJDIR)/test_tokenizer
# OBJDIR_TPAR := $(OBJDIR)/test_parser
# OBJDIR_TEXP := $(OBJDIR)/test_expander
# OBJDIR_TEXEC := $(OBJDIR)/test_exec

# OBJS_MINI := $(SRC_MINISHELL:%.c=$(OBJDIR_MINI)/%.o)
# OBJS_TLEX := $(SRC_TEST_LEX:%.c=$(OBJDIR_TLEX)/%.o)
# OBJS_TTOK := $(SRC_TEST_TOK:%.c=$(OBJDIR_TTOK)/%.o)
# OBJS_TPAR := $(SRC_TEST_PAR:%.c=$(OBJDIR_TPAR)/%.o)
# OBJS_TEXP := $(SRC_TEST_EXP:%.c=$(OBJDIR_TEXP)/%.o)
# OBJS_TEXEC := $(SRC_TEST_EXEC:%.c=$(OBJDIR_TEXEC)/%.o)

# DEPS := $(OBJS_MINI:.o=.d) $(OBJS_TLEX:.o=.d) $(OBJS_TTOK:.o=.d) $(OBJS_TPAR:.o=.d) $(OBJS_TEXP:.o=.d) $(OBJS_TEXEC:.o=.d)

# .PHONY: all clean fclean re test-lexer test-tokenizer test-parser test-expander test-exec

# all: $(NAME)

# $(LIBFT):
# 	$(MAKE) -C $(LIBFT_DIR)

# $(NAME): $(OBJS_MINI)
# 	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# test-lexer: $(NAME_LEX)
# $(NAME_LEX): $(OBJS_TLEX)
# 	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# test-tokenizer: $(NAME_TOK)
# $(NAME_TOK): $(OBJS_TTOK)
# 	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# test-parser: $(NAME_PAR)
# $(NAME_PAR): $(OBJS_TPAR)
# 	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# test-expander: $(NAME_EXP)
# $(NAME_EXP): $(OBJS_TEXP)
# 	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# test-exec: $(NAME_EXEC)
# $(NAME_EXEC): $(OBJS_TEXEC)
# 	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# $(OBJDIR_MINI)/%.o: %.c
# 	@mkdir -p $(dir $@)
# 	$(CC) $(CFLAGS) -c $< -o $@
# $(OBJDIR_TLEX)/%.o: %.c
# 	@mkdir -p $(dir $@)
# 	$(CC) $(CFLAGS) -c $< -o $@
# $(OBJDIR_TTOK)/%.o: %.c
# 	@mkdir -p $(dir $@)
# 	$(CC) $(CFLAGS) -c $< -o $@
# $(OBJDIR_TPAR)/%.o: %.c
# 	@mkdir -p $(dir $@)
# 	$(CC) $(CFLAGS) -c $< -o $@
# $(OBJDIR_TEXP)/%.o: %.c
# 	@mkdir -p $(dir $@)
# 	$(CC) $(CFLAGS) -c $< -o $@
# $(OBJDIR_TEXEC)/%.o: %.c
# 	@mkdir -p $(dir $@)
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm -rf $(OBJDIR)
# 	$(MAKE) -C $(LIBFT_DIR) clean

# fclean: clean
# 	rm -f $(NAME) $(NAME_LEX) $(NAME_TOK) $(NAME_PAR) $(NAME_EXP) $(NAME_EXEC)
# 	$(MAKE) -C $(LIBFT_DIR) fclean

# re: fclean all

# -include $(DEPS)

NAME := minishell
NAME_LEX := test_lexer
NAME_TOK := test_tokenizer
NAME_PAR := test_parser
NAME_EXP := test_expander
NAME_EXEC := test_exec

CC := cc
CFLAGS := -Wall -Wextra -Werror -MMD -MP -I. -ILibft
LDFLAGS := -lreadline

# Libft
LIBFT_DIR := Libft
LIBFT := $(LIBFT_DIR)/libft.a

SRC_LEXER := 1.lexer/char.c 1.lexer/utils.c
SRC_TOKENIZER := 2.tokenizer/tokenize.c 2.tokenizer/utils.c
SRC_PARSER := 3.parser/init.c 3.parser/free.c 3.parser/utils.c 3.parser/parse_tokens.c 3.parser/cmd.c
SRC_EXPANDER := 0.env/env.c  0.env/utils.c 4.expander/expander.c 4.expander/utils.c
SRC_EXEC := 5.exec/exec.c 5.exec/heredoc.c 5.exec/redir.c

SRC_MINISHELL := minishell.c $(SRC_LEXER) $(SRC_TOKENIZER) $(SRC_PARSER)
SRC_TEST_LEX := tests/lexer.c $(SRC_LEXER)
SRC_TEST_TOK := tests/tokenizer.c $(SRC_LEXER) $(SRC_TOKENIZER)
SRC_TEST_PAR := tests/parser.c $(SRC_LEXER) $(SRC_TOKENIZER) $(SRC_PARSER)
SRC_TEST_EXP := tests/test_expander.c $(SRC_LEXER) $(SRC_TOKENIZER) $(SRC_PARSER) $(SRC_EXPANDER)
SRC_TEST_EXEC := tests/exec.c $(SRC_LEXER) $(SRC_TOKENIZER) $(SRC_PARSER) $(SRC_EXPANDER) $(SRC_EXEC)

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

all: $(NAME)

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