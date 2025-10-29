/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 16:21:44 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/29 16:21:45 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* expansion.c                                        :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/* +#+           */
/* Created: 2025/10/29 16:30:00 by ParceiroProg     #+#    #+#             */
/* Updated: 2025/10/29 16:30:00 by ParceiroProg     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Retorna 1 se 'c' for um caractere válido para um nome de variável
 * (alfanumérico ou underscore).
 */
static int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/*
 * Procura o valor da variável 'name' na lista de ambiente.
 * Trata o caso especial "$?".
 */
static char	*get_var_value(char *name, t_env *env)
{
	t_env	*var;

	if (ft_strcmp(name, "?") == 0)
	{
		var = find_env(env, "?");
		if (var)
			return (ft_strdup(var->value));
		else
			return (ft_strdup("0")); // Fallback, embora devêssemos sempre ter
	}
	
	var = find_env(env, name);
	if (var && var->value)
		return (ft_strdup(var->value));
	
	// Se a variável não existe (ex: $VARIAVEL_INEXISTENTE), retorna string vazia
	return (ft_strdup(""));
}

/*
 * Processa um '$'. Encontra o nome da variável (ex: $USER -> "USER"),
 * obtém o seu valor (ex: "vinpache") e anexa-o a 'new_str'.
 * Retorna o novo índice 'i' (após o nome da variável).
 */
static int	handle_variable(char *str, int i, char **new_str, t_env *env)
{
	int		start;
	char	*name;
	char	*value;

	i++; // Pula o '$'
	if (str[i] == '?')
	{
		name = ft_strdup("?");
		i++; // Pula o '?'
	}
	else
	{
		start = i;
		while (is_valid_var_char(str[i]))
			i++;
		name = ft_substr(str, start, i - start);
	}
	
	value = get_var_value(name, env);
	free(name);
	
	*new_str = ft_strjoin_free(*new_str, value); // Anexa o valor
	free(value);
	
	return (i);
}

/*
 * Esta é a função principal da expansão.
 * Recebe uma string (com aspas) e retorna uma nova string (sem aspas
 * e com variáveis expandidas).
 */
static char	*expand_and_remove_quotes(char *str, t_env *env)
{
	char	*new_str;
	char	quote_char;
	int		i;

	new_str = ft_strdup(""); // String de resultado, começa vazia
	quote_char = 0; // 0 = sem aspas, '\'' = em aspas simples, '"' = em aspas duplas
	i = 0;
	while (str[i])
	{
		if (quote_char == 0 && (str[i] == '\'' || str[i] == '"'))
		{
			quote_char = str[i]; // Entra no modo "dentro de aspas"
			i++;
		}
		else if (quote_char != 0 && str[i] == quote_char)
		{
			quote_char = 0; // Sai do modo "dentro de aspas"
			i++;
		}
		// $ só expande se NÃO estiver em aspas simples [cite: 114, 115]
		else if (str[i] == '$' && quote_char != '\'')
		{
			i = handle_variable(str, i, &new_str, env);
		}
		else
		{
			// Anexa o caractere atual à nova string
			char c_str[2] = {str[i], '\0'};
			new_str = ft_strjoin_free(new_str, c_str);
			i++;
		}
	}
	free(str); // Liberta a string original (com aspas)
	return (new_str); // Retorna a nova string (expandida e sem aspas)
}

/*
 * Função de entrada. Itera por todos os comandos e todos os argumentos,
 * aplicando a expansão.
 */
void	expand_commands(t_command *cmds, t_env *env)
{
	t_command	*cmd_ptr;
	t_redirect	*redir_ptr;
	int			i;

	cmd_ptr = cmds;
	while (cmd_ptr)
	{
		// 1. Expande os argumentos (cmd->args)
		i = 0;
		while (cmd_ptr->args && cmd_ptr->args[i])
		{
			cmd_ptr->args[i] = expand_and_remove_quotes(cmd_ptr->args[i], env);
			i++;
		}
		
		// 2. Expande os nomes de ficheiro nos redirecionamentos
		redir_ptr = cmd_ptr->redirects;
		while (redir_ptr)
		{
			// NOTA: 'Heredoc' (<<) não deve ter o delimitador expandido! [cite: 119]
			if (redir_ptr->type != R_HEREDOC)
			{
				redir_ptr->file = expand_and_remove_quotes(redir_ptr->file, env);
			}
			redir_ptr = redir_ptr->next;
		}
		cmd_ptr = cmd_ptr->next;
	}
}