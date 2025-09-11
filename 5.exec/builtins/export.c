#include "../../minishell.h"

static int is_var_start(int c){ return (c=='_'||(c>='A'&&c<='Z')||(c>='a'&&c<='z')); }
static int is_var_continue(int c){ return is_var_start(c)||(c>='0'&&c<='9'); }

static int is_valid_ident(const char *s)
{
    size_t i = 0;
    if (!s || !is_var_start(s[i])) return 0;
    for (i = 1; s[i] && s[i] != '='; ++i)
        if (!is_var_continue(s[i])) return 0;
    return 1;
}

static int setenv_in_vec(char ***penv, const char *name, const char *value)
{
    char **env = *penv; int n = 0; while (env && env[n]) n++;
    size_t ln = strlen(name);
    for (int i = 0; i < n; ++i){
        if (strncmp(env[i], name, ln) == 0 && env[i][ln] == '='){
            char *nv = malloc(ln + 1 + strlen(value) + 1);
            if (!nv) return 0;
            sprintf(nv, "%s=%s", name, value);
            free(env[i]); env[i] = nv; return 1;
        }
    }
    char **grown = realloc(env, (n + 2) * sizeof(*grown));
    if (!grown) return 0;
    grown[n] = malloc(ln + 1 + strlen(value) + 1);
    if (!grown[n]) return 0;
    sprintf(grown[n], "%s=%s", name, value);
    grown[n+1] = NULL;
    *penv = grown;
    return 1;
}

static void print_export_line(const char *entry)
{
    const char *eq = strchr(entry, '=');
    if (!eq){
        dprintf(STDOUT_FILENO, "declare -x %s\n", entry);
    } else {
        size_t ln = (size_t)(eq - entry);
        write(STDOUT_FILENO, "declare -x ", 12);
        write(STDOUT_FILENO, entry, ln);
        write(STDOUT_FILENO, "=\"", 2);
        write(STDOUT_FILENO, eq + 1, strlen(eq + 1));
        write(STDOUT_FILENO, "\"\n", 2);
    }
}

int builtin_export(t_command *cmd, t_shell *sh)
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
    if (argv[0] && strcmp(argv[0], "export") == 0) idx = 1;

    if (!argv[idx]) {
        for (int k = 0; sh->env && sh->env[k]; ++k)
            print_export_line(sh->env[k]);
        free(argv);
        return 0;
    }

    for (; argv[idx]; ++idx)
    {
        if (!is_valid_ident(argv[idx])) {
            dprintf(STDERR_FILENO, "minishell: export: `%s': not a valid identifier\n", argv[idx]);
            status = 1;
            continue;
        }
        const char *eq = strchr(argv[idx], '=');
        if (eq){
            size_t ln = (size_t)(eq - argv[idx]);
            char *name = strndup(argv[idx], ln);
            const char *val = eq + 1;
            if (!name || !setenv_in_vec(&sh->env, name, val)) status = 1;
            free(name);
        } else {
            if (!setenv_in_vec(&sh->env, argv[idx], "")) status = 1;
        }
    }
    free(argv);
    return status;
}
