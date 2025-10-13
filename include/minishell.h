#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

typedef struct s_cmd
{
    char            **argv;
    char            **redirs;
    int             in_fd;
    int             out_fd;
    struct s_cmd    *next;
}   t_cmd;

extern volatile sig_atomic_t g_sig;

/* main.c */
void    init_minishell(char **envp);
void    prompt_loop(char **envp);
void    setup_signals(void);
void    sigint_handler(int sig);

/* parser.c */
char    **split_tokens(const char *line);
char    *expand_vars(const char *token, char **envp, int last_status);
int     build_commands(char **tokens, t_cmd **cmds);
void    free_tokens(char **tokens);
int     is_quoted(const char *s);

/* executor.c */
int     execute_pipeline(t_cmd *cmds, char ***envp, int *last_status);
int     execute_child(t_cmd *cmd, char **envp);
int     find_and_exec_command(t_cmd *cmd, char **envp);
int     setup_fds_for_cmd(t_cmd *cmd, int in_fd, int out_fd);
int     handle_redirections(t_cmd *cmd);

/* builtins.c */
int     builtin_echo(char **argv);
int     builtin_cd(char **argv, char ***envp);
int     builtin_pwd(void);
int     builtin_exit(char **argv);
int     builtin_env(char **envp);

/* env_manage.c */
int     builtin_export(char **argv, char ***envp);
int     builtin_unset(char **argv, char ***envp);
char    **dup_env(char **envp);
char    *get_env_value(char **envp, const char *name);
int     set_env_value(char ***envp, const char *name, const char *value);

#endif
