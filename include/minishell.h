#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
#include <sys/wait.h> // Para wait() e waitpid()
#include <fcntl.h>    // Para open() e as constantes O_*
# include <termios.h> // <-- ADICIONE ESTE

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
// CHANGE THIS LINE:
void    init_minishell(void); // Remove 'char **envp'
void    prompt_loop(char **envp, struct termios *original_termios); // <-- Adicionar termios
void    setup_signals(void);
void    sigint_handler(int sig);

// ... (rest of the file)
/* parser.c */
char    **split_tokens(const char *line);
char    *expand_vars(const char *token, char **envp, int last_status);
// ATUALIZE A LINHA ABAIXO
int     build_commands(char **tokens, t_cmd **cmds, char **envp, int last_status);
void    free_tokens(char **tokens);
int     is_quoted(const char *s);

/* executor.c */
int     execute_pipeline(t_cmd *cmds, char ***envp, int *last_status, struct termios *termios);
int     execute_child(t_cmd *cmd, char **envp, int in_fd, int out_fd);
int     find_and_exec_command(t_cmd *cmd, char **envp);
int     setup_fds_for_cmd(t_cmd *cmd, int in_fd, int out_fd);
int     handle_redirections(t_cmd *cmd);

/* builtins.c */
int     builtin_echo(char **argv);
int     builtin_cd(char **argv, char ***envp);
int     builtin_pwd(void);
// CHANGE THIS LINE:
int     builtin_exit(char **argv, char **envp, struct termios *termios);
int     builtin_env(char **envp);

/* env_manage.c */
int     builtin_export(char **argv, char ***envp);
int     builtin_unset(char **argv, char ***envp);
char    **dup_env(char **envp);
char    *get_env_value(char **envp, const char *name);
int     set_env_value(char ***envp, const char *name, const char *value);

#endif
