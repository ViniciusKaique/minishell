#include "minishell.h"

char **split_tokens(const char *line)
{
    (void)line;
    return (NULL);
}

char *expand_vars(const char *token, char **envp, int last_status)
{
    (void)token; (void)envp; (void)last_status;
    return (NULL);
}

int build_commands(char **tokens, t_cmd **cmds)
{
    (void)tokens; (void)cmds;
    return (0);
}

void free_tokens(char **tokens)
{
    (void)tokens;
}

int is_quoted(const char *s)
{
    (void)s;
    return (0);
}
