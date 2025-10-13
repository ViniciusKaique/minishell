#include "minishell.h"

int builtin_echo(char **argv)
{
    (void)argv;
    return (0);
}

int builtin_cd(char **argv, char ***envp)
{
    (void)argv; (void)envp;
    return (0);
}

int builtin_pwd(void)
{
    return (0);
}

int builtin_exit(char **argv)
{
    (void)argv;
    exit(0);
}

int builtin_env(char **envp)
{
    (void)envp;
    return (0);
}
