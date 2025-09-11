#include "../../minishell.h"

static int setenv_in_vec(char ***penv, const char *name, const char *value)
{
    char **env = *penv;
    int n = 0; while (env && env[n]) n++;
    size_t ln = strlen(name);

    for (int i = 0; i < n; ++i) {
        if (strncmp(env[i], name, ln) == 0 && env[i][ln] == '=') {
            char *nv = malloc(ln + 1 + strlen(value) + 1);
            if (!nv) return 0;
            sprintf(nv, "%s=%s", name, value);
            free(env[i]);
            env[i] = nv;
            return 1;
        }
    }
    char **grown = realloc(env, (n + 2) * sizeof(*grown));
    if (!grown) return 0;
    grown[n] = malloc(ln + 1 + strlen(value) + 1);
    if (!grown[n]) return 0;
    sprintf(grown[n], "%s=%s", name, value);
    grown[n + 1] = NULL;
    *penv = grown;
    return 1;
}

static const char *get_env(char **env, const char *name)
{
    return find_variable_in_env(env, (char*)name); 
}

int builtin_cd(t_command *cmd, t_shell *sh)
{
    t_element *e = cmd->element;
    if (e && e->kind == ARG) e = e->next;

    const char *target = NULL;
    if (!e || e->kind != ARG || !e->u_.arg || !e->u_.arg->str || !*e->u_.arg->str) {
        target = get_env(sh->env, "HOME");
        if (!target) {
            fprintf(stderr, "minishell: cd: HOME not set\n");
            return 1;
        }
    } else {
        target = e->u_.arg->str;
    }

    char oldpwd[4096];
    if (!getcwd(oldpwd, sizeof oldpwd)) oldpwd[0] = '\0';

    if (chdir(target) != 0) {
        perror("cd");
        return 1;
    }

    char newpwd[4096];
    if (!getcwd(newpwd, sizeof newpwd)) {
        return 0;
    }

    setenv_in_vec(&sh->env, "OLDPWD", oldpwd);
    setenv_in_vec(&sh->env, "PWD", newpwd);
    return 0;
}
