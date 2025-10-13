#include "minishell.h"

int execute_pipeline(t_cmd *cmds, char ***envp, int *last_status)
{
    (void)cmds; (void)envp; (void)last_status;
    return (0);
}

int execute_child(t_cmd *cmd, char **envp)
{
    (void)cmd; (void)envp;
    return (0);
}

int find_and_exec_command(t_cmd *cmd, char **envp)
{
    (void)cmd; (void)envp;
    return (0);
}

int setup_fds_for_cmd(t_cmd *cmd, int in_fd, int out_fd)
{
    (void)cmd; (void)in_fd; (void)out_fd;
    return (0);
}

int handle_redirections(t_cmd *cmd)
{
    (void)cmd;
    return (0);
}
