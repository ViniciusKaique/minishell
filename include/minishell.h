#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>

// Tipos de redirecionamento (mandatório)
typedef enum e_redirect_type
{
	R_OUT_TRUNC,  // >
	R_OUT_APPEND, // >>
	R_IN,         // <
	R_HEREDOC     // <<
}	t_redirect_type;

typedef struct s_redirect
{
	t_redirect_type type;    // qual redirecionamento é (>, >>, <, <<)
	char *file;              // nome do arquivo (ou delimitador no heredoc)
	struct s_redirect *next; // próximo redirecionamento do mesmo comando
}	t_redirect;

typedef struct s_command
{
	char **args;           // argv do comando: ["ls","-la",NULL]
	t_redirect *redirects; // lista ligada de redirecionamentos
	struct s_command *next;    // próximo comando no pipeline
}	t_command;

t_command *new_command(void);
void	add_command_back(t_command **list, t_command *new);

t_redirect	*new_redirect(t_redirect_type type, char *file);
void	add_redirect_back(t_redirect **list, t_redirect *new);

#endif
