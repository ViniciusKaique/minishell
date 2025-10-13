#include "minishell.h"

int builtin_export(char **argv, char ***envp)
{
    (void)argv; (void)envp;
    return (0);
}

int builtin_unset(char **argv, char ***envp)
{
    (void)argv; (void)envp;
    return (0);
}

char **dup_env(char **envp)
{
    (void)envp;
    return (NULL);
}

char *get_env_value(char **envp, const char *name)
{
    (void)envp; (void)name;
    return (NULL);
}

int set_env_value(char ***envp, const char *name, const char *value)
{
    (void)envp; (void)name; (void)value;
    return (0);
}
