#include "../minishell.h"
#include <errno.h>
#include <fcntl.h>

static char *join_path(const char *dir, const char *cmd)
{
    size_t ld = strlen(dir), lc = strlen(cmd);
    char *p = malloc(ld + 1 + lc + 1);
    if (!p) return NULL;
    memcpy(p, dir, ld); p[ld] = '/';
    memcpy(p + ld + 1, cmd, lc);
    p[ld + 1 + lc] = '\0';
    return p;
}

static char *resolve_in_path(const char *cmd, char **env)
{
    char *path = find_variable_in_env(env, "PATH");
    if (!path || !*path) return NULL;

    char *copy = strdup(path);
    if (!copy) return NULL;

    char *save = copy;
    char *dir = copy;
    char *sep;
    char *candidate = NULL;

    while (dir) {
        sep = strchr(dir, ':');
        if (sep) *sep = '\0';
        if (*dir) {
            candidate = join_path(dir, cmd);
            if (!candidate) { free(save); return NULL; }
            if (access(candidate, X_OK) == 0) {
                free(save);
                return candidate;
            }
            free(candidate);
        }
        if (!sep) break;
        dir = sep + 1;
    }
    free(save);
    return NULL;
}

static int exec_with_path(char **argv, t_shell *sh)
{
    if (!argv || !argv[0]) { errno = EINVAL; return -1; }

    if (strchr(argv[0], '/')) {          
        execve(argv[0], argv, sh->env);
        return -1;
    }
    char *full = resolve_in_path(argv[0], sh->env);
    if (full) {
        execve(full, argv, sh->env);
        free(full);
        return -1;
    }
    errno = ENOENT;
    return -1;
}

static int is_simple_builtin_type(t_type_cmd c)
{
    return (c == T_ECHO || c == T_PWD || c == T_CD ||
            c == T_EXPORT || c == T_UNSET || c == T_ENV || c == T_EXIT);
}

int is_builtin_cmd(t_type_cmd c) { return is_simple_builtin_type(c); }

int exec_builtin(t_command *cmd, t_shell *sh)
{
    if (cmd->cmd == T_ECHO)   return builtin_echo(cmd, sh);
    if (cmd->cmd == T_PWD)    return builtin_pwd(cmd, sh);
    if (cmd->cmd == T_CD)     return builtin_cd(cmd, sh);
    if (cmd->cmd == T_EXPORT) return builtin_export(cmd, sh);
    if (cmd->cmd == T_UNSET)  return builtin_unset(cmd, sh);
    if (cmd->cmd == T_ENV)    return builtin_env(cmd, sh);
    if (cmd->cmd == T_EXIT)   return builtin_exit(cmd, sh);
    return 127;
}

static char **build_argv(const t_command *cmd)
{
    size_t argc = 0;
    for (t_element *e = cmd->element; e; e = e->next)
        if (e->kind == ARG) argc++;

    if (argc == 0) return NULL;

    char **argv = calloc(argc + 1, sizeof(char*));
    if (!argv) return NULL;

    size_t i = 0;
    for (t_element *e = cmd->element; e; e = e->next)
        if (e->kind == ARG)
            argv[i++] = strdup(e->u_.arg->str);
    argv[i] = NULL;
    return argv;
}

static void free_argv(char **argv) {
    if (!argv) return;
    for (size_t i = 0; argv[i]; ++i) free(argv[i]);
    free(argv);
}

static int is_last_cmd(t_command *cmd) { return cmd->next == NULL; }

static pid_t spawn_one(t_command *cmd, int prev_read_fd, int pipe_out_fd, t_shell *sh)
{
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return -1; }

    if (pid == 0) {

        if (prev_read_fd >= 0) {
            if (dup2(prev_read_fd, STDIN_FILENO) < 0) { perror("dup2"); _exit(1); }
        }
        t_ios ios;
        if (!collect_redirs_fds(cmd->element, &ios)) _exit(1);
        if (pipe_out_fd >= 0) {
            if (dup2(pipe_out_fd, STDOUT_FILENO) < 0) { perror("dup2"); _exit(1); }
        }
        if (!apply_redirs(&ios)) _exit(1);
        if (prev_read_fd >= 0) close(prev_read_fd);
        if (pipe_out_fd >= 0) close(pipe_out_fd);
        close_redirs(&ios);
        if (is_builtin_cmd(cmd->cmd)) {
            int code = exec_builtin(cmd, sh);
            _exit(code);
        } else {
            char **argv = build_argv(cmd);
            if (!argv) _exit(0); 
            if (exec_with_path(argv, sh) < 0) {
                perror(argv[0]);
                free_argv(argv);
                _exit(127);
            }
        }
    }
    return pid; 
}

int run_pipeline(t_command *cmd_list, t_shell *sh)
{
    if (cmd_list && !cmd_list->next && is_builtin_cmd(cmd_list->cmd))
    {
        int saved_in  = dup(STDIN_FILENO);
        int saved_out = dup(STDOUT_FILENO);
        if (saved_in < 0 || saved_out < 0) { perror("dup"); return 1; }
        t_ios ios;
        if (!collect_redirs_fds(cmd_list->element, &ios)) {
            if (saved_in  >= 0) close(saved_in);
            if (saved_out >= 0) close(saved_out);
            return 1;
        }
        if (!apply_redirs(&ios)) {
            close_redirs(&ios);
            if (saved_in  >= 0) close(saved_in);
            if (saved_out >= 0) close(saved_out);
            return 1;
        }
        close_redirs(&ios);
        int code = exec_builtin(cmd_list, sh);
        if (dup2(saved_in, STDIN_FILENO)   < 0) perror("dup2");
        if (dup2(saved_out, STDOUT_FILENO) < 0) perror("dup2");
        if (saved_in  >= 0) close(saved_in);
        if (saved_out >= 0) close(saved_out);

        sh->last_exit = code;
        return code;
    }
    int   prev_read = -1;
    int   pfd[2];
    pid_t pids[256];
    int   n = 0;

    for (t_command *cmd = cmd_list; cmd; cmd = cmd->next) {
        int out_write = -1;

        if (!is_last_cmd(cmd)) {
            if (pipe(pfd) < 0) { perror("pipe"); return 1; }
            out_write = pfd[1];
        }

        pid_t pid = spawn_one(cmd, prev_read, out_write, sh);
        if (pid < 0) return 1;
        pids[n++] = pid;
        if (prev_read >= 0) { close(prev_read); prev_read = -1; }
        if (!is_last_cmd(cmd)) {
            close(out_write);
            prev_read = pfd[0];
        }
    }

    int status = 0, last_status = 0;
    for (int i = 0; i < n; ++i) {
        if (waitpid(pids[i], &status, 0) > 0) last_status = status;
    }
    if (WIFEXITED(last_status))       sh->last_exit = WEXITSTATUS(last_status);
    else if (WIFSIGNALED(last_status)) sh->last_exit = 128 + WTERMSIG(last_status);
    else                               sh->last_exit = 1;

    return sh->last_exit;
}
