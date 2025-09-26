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

SRC_TEST_EXEC := main.c \
1.lexer/char.c 1.lexer/utils.c 1.lexer/init.c \
6.free/free.c 6.free/free2.c 6.free/error.c \
2.tokenizer/tokenize.c 2.tokenizer/utils.c 2.tokenizer/local_utils.c \
3.parser/init.c 3.parser/utils.c 3.parser/parse_tokens.c 3.parser/cmd.c 3.parser/add.c \
0.env/env.c 0.env/utils.c 0.env/signals.c 0.env/shlvl.c \
4.expander/expander.c 4.expander/utils4.c 4.expander/utils0.c 4.expander/utils1.c 4.expander/utils2.c 4.expander/utils3.c \
5.exec/exec.c 5.exec/heredoc.c 5.exec/redir.c \
5.exec/builtins/echo.c 5.exec/builtins/cd.c 5.exec/builtins/pwd.c 5.exec/builtins/export.c 5.exec/builtins/unset.c \
5.exec/builtins/env.c 5.exec/builtins/exit.c \
5.exec/norme/redir_norme.c 5.exec/norme/redir_norme2.c 5.exec/norme/unset_norme.c 5.exec/norme/exit_norme.c \
5.exec/norme/export_norme.c 5.exec/norme/export_norme2.c 5.exec/norme/cd_norme.c \
5.exec/norme/exec_norme.c 5.exec/norme/exec_norme2.c 5.exec/norme/exec_norme3.c 5.exec/norme/exec_norme4.c \
5.exec/norme/exec_norme5.c 5.exec/norme/exec_norme6.c 5.exec/norme/exec_norme7.c


OBJDIR := obj
OBJDIR_TEXEC := $(OBJDIR)/test_exec
OBJS_TEXEC := $(SRC_TEST_EXEC:%.c=$(OBJDIR_TEXEC)/%.o)

DEPS := $(OBJS_TEXEC:.o=.d)

.PHONY: all clean fclean re test-lexer test-tokenizer test-parser test-expander test-exec

all: $(NAME_EXEC)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

test-exec: $(NAME_EXEC)
$(NAME_EXEC): $(LIBFT) $(OBJS_TEXEC)
	$(CC) $(CFLAGS) $(OBJS_TEXEC) $(LIBFT) -o $@ $(LDFLAGS)

$(OBJDIR_TEXEC)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME_EXEC)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

-include $(DEPS)
