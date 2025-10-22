#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

/*─────────────────────────────*/
/*         ENUMS               */
/*─────────────────────────────*/

// Tipos de redirecionamento (>, >>, <, <<)
typedef enum e_redirect_type
{
	R_OUT_TRUNC,   // >
	R_OUT_APPEND,  // >>
	R_IN,          // <
	R_HEREDOC      // <<
}	t_redirect_type;

// Tipos de tokens (para o lexer)
typedef enum e_token_type
{
	T_WORD,          // palavra normal
	T_PIPE,          // |
	T_REDIR_IN,      // <
	T_REDIR_OUT,     // >
	T_REDIR_APPEND,  // >>
	T_HEREDOC        // <<
}	t_token_type;

/*─────────────────────────────*/
/*         STRUCTS              */
/*─────────────────────────────*/

// Redirecionamento
typedef struct s_redirect
{
	t_redirect_type	type;
	char			*file;
	struct s_redirect	*next;
}	t_redirect;

// Comando (um nó do pipeline)
typedef struct s_command
{
	char			**args;        // argv: ["ls", "-la", NULL]
	t_redirect		*redirects;    // redirecionamentos associados
	struct s_command	*next;     // próximo comando no pipeline
}	t_command;

// Token (resultado da tokenização do input)
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

// Variável de ambiente
typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

/*─────────────────────────────*/
/*        FUNÇÕES ÚTEIS        */
/*─────────────────────────────*/

// Criação
t_command	*new_command(void);
t_redirect	*new_redirect(t_redirect_type type, char *file);
t_token		*new_token(t_token_type type, char *value);
t_env		*new_env(char *name, char *value);

// Encadeamento
void	add_command_back(t_command **list, t_command *new);
void	add_redirect_back(t_redirect **list, t_redirect *new);
void	add_token_back(t_token **list, t_token *new);
void	add_env_back(t_env **list, t_env *new);

// Liberação
void	free_redirects(t_redirect *redir);
void	free_commands(t_command *cmd);
void	free_tokens(t_token *tok);
void	free_env(t_env *env);

#endif
