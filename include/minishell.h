/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* minishell.h                                        :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/10/22 17:00:00 by vinpache          #+#    #+#             */
/* Updated: 2025/10/25 17:30:00 by vinpache         ###   ########.fr       */
/* */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* --- Bibliotecas Autorizadas --- */
# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <errno.h>

/* --- Estruturas e Enums --- */

/**
 * @brief Tipos de tokens identificados pelo lexer.
 */
typedef enum e_token_type
{
	T_WORD,			/* Palavra, comando, argumento, nome de arquivo */
	T_PIPE,			/* | */
	T_REDIR_IN,		/* < */
	T_REDIR_OUT,	/* > */
	T_HEREDOC,		/* << */
	T_REDIR_APPEND	/* >> */
}	t_token_type;

/**
 * @brief Tipos de redirecionamento para o executor.
 */
typedef enum e_redirect_type
{
	R_IN,			/* < */
	R_OUT_TRUNC,	/* > */
	R_OUT_APPEND,	/* >> */
	R_HEREDOC		/* << */
}	t_redirect_type;

/**
 * @brief Nó da lista encadeada de variáveis de ambiente.
 */
typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

/**
 * @brief Nó da lista encadeada de tokens (saída do lexer).
 */
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

/**
 * @brief Nó da lista encadeada de redirecionamentos (parte do t_command).
 */
typedef struct s_redirect
{
	t_redirect_type	type;
	char			*file;
	struct s_redirect	*next;
}	t_redirect;

/**
 * @brief Nó da lista encadeada de comandos (saída do parser).
 * Representa um único comando em um pipeline.
 */
typedef struct s_command
{
	char				**args;		/* Array de strings (comando + argumentos) */
	t_redirect			*redirects;	/* Lista de redirecionamentos */
	struct s_command	*next;		/* Próximo comando no pipeline (após |) */
}	t_command;

/* --- Protótipos de Funções --- */

/* command_utils.c */
t_command	*new_command(void);
void		add_command_back(t_command **list, t_command *new);

/* envp_utils.c */
t_env		*new_env(char *name, char *value);
void		add_env_back(t_env **list, t_env *new);
char	**env_to_array(t_env *env);

/* --- Funções de inicialização e execução --- */
t_env   *init_env(char **envp);
void    execute_commands(t_command *cmds, t_env **env);
void		exec(char **args, char **envp);

/* free_utils.c */
void		free_redirects(t_redirect *redir);
void		free_tokens(t_token *tok);
void		free_env(t_env *env);
void		free_commands(t_command *cmd);
void		free_split(char **split);

/* lexer.c */
t_token		*tokenize(char *line);

/* parser.c */
t_command	*parse(t_token *tokens);

/* path_utils.c */
char		*find_command_path(char *cmd);

/* redirect_utils.c */
t_redirect	*new_redirect(t_redirect_type type, char *file);
void		add_redirect_back(t_redirect **list, t_redirect *new);

/* token_utils.c */
t_token		*new_token(t_token_type type, char *value);
void		add_token_back(t_token **list, t_token *new);

// --- builtins ---
int	builtin_echo(char **args);
int	builtin_cd(char **args, t_env **env);
int	builtin_pwd(void);
int	builtin_export(char **args, t_env **env);
int	builtin_unset(char **args, t_env **env);
int	builtin_env(t_env *env);
int	builtin_exit(char **args);
int	is_builtin(char *cmd);
int	exec_builtin(char **args, t_env **env);
int	is_valid_name(const char *s);
t_env	*find_env(t_env *env, const char *name);
int	set_env_kv(t_env **env, const char *name, const char *value);


#endif