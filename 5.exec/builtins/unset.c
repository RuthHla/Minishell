#include "../../minishell.h"

static int is_var_start(int c){ return (c=='_'||(c>='A'&&c<='Z')||(c>='a'&&c<='z')); }
static int is_var_continue(int c){ return is_var_start(c)||(c>='0'&&c<='9'); }

static int is_valid_ident(const char *s)
{
    size_t i = 0;
    if (!s || !is_var_start(s[i])) return 0;
    for (i = 1; s[i]; ++i)
        if (!is_var_continue(s[i])) return 0;
    return 1;
}

static void remove_env_var(char ***penv, const char *name)
{
    char **env = *penv; if (!env) return;
    size_t ln = strlen(name);
    int w = 0;
    for (int r = 0; env[r]; ++r){
        if (!(strncmp(env[r], name, ln) == 0 && env[r][ln] == '=')){
            env[w++] = env[r];
        } else {
            free(env[r]); // supprime l'entrée
        }
    }
    env[w] = NULL;
}

int builtin_unset(t_command *cmd, t_shell *sh)
{
    size_t argc = 0;
    for (t_element *e = cmd->element; e; e = e->next) if (e->kind == ARG) argc++;
    char **argv = calloc(argc + 1, sizeof(char*));
    if (!argv) return 1;
    size_t i = 0; for (t_element *e = cmd->element; e; e = e->next)
        if (e->kind == ARG) argv[i++] = e->u_.arg->str;
    argv[i] = NULL;

    int status = 0;
    int idx = 0;
    if (argv[0] && strcmp(argv[0], "unset") == 0) idx = 1;

    for (; argv[idx]; ++idx){
        if (!is_valid_ident(argv[idx])){
            dprintf(STDERR_FILENO, "minishell: unset: `%s': not a valid identifier\n", argv[idx]);
            status = 1;
            continue;
        }
        remove_env_var(&sh->env, argv[idx]);
    }
    free(argv);
    return status;
}
