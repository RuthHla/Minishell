#include "../minishell.h"
#include <limits.h>
#include <errno.h>

static int is_numeric_word(const char *s)
{
    if (!s || !*s) return 0;
    const char *p = s;
    if (*p == '+' || *p == '-') p++;
    if (!*p) return 0;
    while (*p) {
        if (*p < '0' || *p > '9') return 0;
        p++;
    }
    return 1;
}

static unsigned char to_exit_u8(const char *s)
{
    // parse en signed long long puis modulo 256
    // (bash fait un wrap 8-bit)
    errno = 0;
    char *end = NULL;
    long long v = strtoll(s, &end, 10);
    (void)end;
    unsigned char code = (unsigned char)v;
    return code;
}

int builtin_exit(t_command *cmd, t_shell *sh)
{
    // Construire argv simple (sans mots vides: pas indispensable ici)
    size_t argc = 0;
    for (t_element *e = cmd->element; e; e = e->next)
        if (e->kind == ARG) argc++;
    char **argv = calloc(argc + 1, sizeof(char*));
    if (!argv) return 1;
    size_t i = 0;
    for (t_element *e = cmd->element; e; e = e->next)
        if (e->kind == ARG) argv[i++] = e->u_.arg->str;
    argv[i] = NULL;

    int idx = 0;
    if (argv[0] && strcmp(argv[0], "exit") == 0) idx = 1;

    dprintf(STDOUT_FILENO, "exit\n");

    if (!argv[idx]) { // exit
        free(argv);
        exit(sh->last_exit);
    }

    if (!is_numeric_word(argv[idx])) {
        dprintf(STDERR_FILENO, "minishell: exit: %s: numeric argument required\n", argv[idx]);
        free(argv);
        exit(2);
    }

    if (argv[idx + 1]) {
        dprintf(STDERR_FILENO, "minishell: exit: too many arguments\n");
        free(argv);
        return 1; // ne pas quitter
    }

    unsigned char code = to_exit_u8(argv[idx]);
    free(argv);
    exit(code);
}