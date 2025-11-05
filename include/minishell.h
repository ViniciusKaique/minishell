/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrito-g <kbrito-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:12:06 by vinpache          #+#    #+#             */
/*   Updated: 2025/11/05 16:29:09 by kbrito-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

# define HEREDOC_TMP_FILE "/tmp/.minishell_heredoc"

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_HEREDOC,
	T_REDIR_APPEND
}								t_token_type;

typedef enum e_redirect_type
{
	R_IN,
	R_OUT_TRUNC,
	R_OUT_APPEND,
	R_HEREDOC
}								t_redirect_type;

typedef struct s_env
{
	char						*name;
	char						*value;
	struct s_env				*next;
}								t_env;

typedef struct s_token
{
	t_token_type				type;
	char						*value;
	int							has_quotes;
	struct s_token				*next;
}								t_token;

typedef struct s_redirect
{
	t_redirect_type				type;
	char						*file;
	struct s_redirect			*next;
}								t_redirect;

typedef struct s_command
{
	char						**args;
	int							*args_has_quotes;
	t_redirect					*redirects;
	struct s_command			*next;
}								t_command;

typedef struct s_pipe_data
{
	int							pipefd[2];
	int							*in_fd;
}								t_pipe_data;


t_command						*parse(t_token *tokens);
t_command						*new_command(void);
void							add_command_back(t_command **list,
									t_command *new);
int								add_arg_to_cmd(t_command *cmd, char *arg_val,
									int has_quotes);
t_command						*create_and_append_cmd(t_command **cmd_list);
t_token							*tokenize(char *line);
t_token							*new_token(t_token_type type, char *value,
									int has_quotes);
void							add_token_back(t_token **list, t_token *new);
void							add_token(t_token **lst, t_token_type type,
									char *value, int has_quotes);
t_redirect						*new_redirect(t_redirect_type type, char *file);
void							add_redirect_back(t_redirect **list,
									t_redirect *new);

void							expand_commands(t_command *cmds, t_env *env);
int								handle_variable(char *str, int i,
									char **new_str, t_env *env);
void							append_char_to_str(char **s, char c);
void							handle_quotes(char c, char *quote_char);
int								is_valid_var_char(char c);
void							expand_redirects(t_command *cmd, t_env *env);
char							*expand_and_remove_quotes(char *str,
									t_env *env);

t_env							*new_env(char *name, char *value);
void							add_env_back(t_env **list, t_env *new);
t_env							*init_env(char **envp);
t_env							*find_env(t_env *env, const char *name);
int								set_env_kv(t_env **env, const char *name,
									const char *value);
char							*get_env_val(t_env *env, const char *name);
int								builtin_env(t_env *env);

int								builtin_echo(char **args);
int								builtin_cd(char **args, t_env **env);
int								builtin_pwd(void);
int								builtin_export(char **args, t_env **env);
int								builtin_unset(char **args, t_env **env);
int								builtin_exit(char **args);
int								is_builtin(char *cmd);
int								exec_builtin(char **args, t_env **env);
int								is_valid_name(const char *s);

int								execute_commands(t_command *cmds, t_env **env);
int								exec_pipeline(t_command *cmds, t_env **env);
int								apply_redirects(t_redirect *redir);
int								handle_heredoc(char *delimiter);
char							*find_command_path(char *cmd);
char							**env_to_array(t_env *env);
void							expand_redirects(t_command *cmd,
									t_env *env);
void							run_external_command(char **args, t_env *env);
int								get_pipeline_exit_status(int status);
void							handle_path_pre_exec_errors(char *path,
									char **args);

extern volatile sig_atomic_t	g_signal_received;
void							setup_child_io_and_signals(int in_fd,
									int out_fd);
void							handle_sigint_prompt(int sig);
void							handle_sigint_heredoc(int sig);
void							setup_signals(void);
void							free_redirects(t_redirect *redir);
void							free_tokens(t_token *tok);
void							free_env(t_env *env);
void							free_commands(t_command *cmd);
void							free_split(char **split);

#endif