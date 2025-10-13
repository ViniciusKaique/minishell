#include "minishell.h"

volatile sig_atomic_t g_sig = 0;

int main(int argc, char **argv, char **envp)
{
    // (void)argc; (void)argv;
    // init_minishell(envp);
    // prompt_loop(envp);
    // return (0);
}

void init_minishell(char **envp)
{
    // (void)envp;
    // setup_signals();
}

void prompt_loop(char **envp)
{
    // char    *line;
    // int     last_status = 0;
    // t_cmd   *cmds;

    // while (1)
    // {
    //     line = readline("minishell$ ");
    //     if (!line)
    //         break ;
    //     if (*line)
    //         add_history(line);
    //     build_commands(split_tokens(line), &cmds);
    //     execute_pipeline(cmds, &envp, &last_status);
    //     free(line);
    //     // free_cmds(cmds); // implementar depois
    // }
}

void setup_signals(void)
{
    // signal(SIGINT, sigint_handler);
    // signal(SIGQUIT, SIG_IGN);
}

void sigint_handler(int sig)
{
    // (void)sig;
    // g_sig = 1;
    // write(1, "\nminishell$ ", 12);
}
