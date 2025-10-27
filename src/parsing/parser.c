/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* parser.c                                           :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/10/25 17:30:00 by vinpache          #+#    #+#             */
/* Updated: 2025/10/25 17:30:00 by vinpache         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Converte um t_token_type em um t_redirect_type.
 */
static t_redirect_type	get_redir_type(t_token_type type)
{
	if (type == T_REDIR_IN)
		return (R_IN);
	if (type == T_REDIR_OUT)
		return (R_OUT_TRUNC);
	if (type == T_REDIR_APPEND)
		return (R_OUT_APPEND);
	return (R_HEREDOC);
}

/**
 * @brief Processa um token de redirecionamento.
 * Cria um nó t_redirect, o anexa ao comando atual e avança o ponteiro
 * de token além do nome do arquivo.
 * Retorna 0 em sucesso, 1 em erro de sintaxe ou alocação.
 */
static int	handle_redirect(t_command *cmd, t_token **tok_ptr)
{
	t_token			*tok;
	t_redirect_type	r_type;
	t_redirect		*redir;
	char			*file;

	tok = *tok_ptr;
	r_type = get_redir_type(tok->type);
	if (!tok->next || tok->next->type != T_WORD)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token", 2);
		return (1);
	}
	file = ft_strdup(tok->next->value);
	if (!file)
		return (1);
	redir = new_redirect(r_type, file);
	if (!redir)
		return (free(file), 1);
	add_redirect_back(&(cmd->redirects), redir);
	*tok_ptr = tok->next->next;
	return (0);
}

/**
 * @brief Adiciona um argumento (string) ao array cmd->args.
 * Realoca o array cmd->args para acomodar a nova string.
 * Retorna 0 em sucesso, 1 em erro de alocação.
 */
static int	add_arg_to_cmd(t_command *cmd, char *arg_val)
{
	char	**new_args;
	char	*arg_copy;
	int		i;

	i = 0;
	while (cmd->args && cmd->args[i])
		i++;
	new_args = ft_calloc(i + 2, sizeof(char *));
	if (!new_args)
		return (1);
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	arg_copy = ft_strdup(arg_val);
	if (!arg_copy)
		return (free(new_args), 1);
	new_args[i] = arg_copy;
	if (cmd->args)
		free(cmd->args);
	cmd->args = new_args;
	return (0);
}

/**
 * @brief Cria um novo t_command e o anexa ao final da lista de comandos.
 */
static t_command	*create_and_append_cmd(t_command **cmd_list)
{
	t_command	*new_cmd;

	new_cmd = new_command();
	if (!new_cmd)
		return (NULL);
	add_command_back(cmd_list, new_cmd);
	return (new_cmd);
}

/**
 * @brief Função principal do parser.
 * Converte a lista de tokens (do lexer) em uma lista encadeada de
 * t_command (pipeline), pronta para o executor.
 */
t_command	*parse(t_token *tokens)
{
	t_command	*cmd_list;
	t_command	*current_cmd;
	t_token		*tok;

	cmd_list = NULL;
	tok = tokens;
	current_cmd = create_and_append_cmd(&cmd_list);
	if (!current_cmd)
		return (NULL);
	while (tok)
	{
		if (tok->type == T_WORD)
		{
			if (add_arg_to_cmd(current_cmd, tok->value))
				return (free_commands(cmd_list), NULL);
			tok = tok->next;
		}
		else if (tok->type == T_PIPE)
		{
			/*
			 * VERIFICAÇÃO DE ERRO DE SINTAXE OBRIGATÓRIA:
			 * 1. Se o pipe for o último token.
			 * 2. Se o próximo token também for um pipe (ex: "||").
			 * 3. Se for o primeiro token (ex: "| ls").
			 */
			if (!tok->next || tok->next->type == T_PIPE || current_cmd->args == NULL)
			{
				ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
				return (free_commands(cmd_list), NULL);
			}
			current_cmd = create_and_append_cmd(&cmd_list);
			if (!current_cmd)
				return (free_commands(cmd_list), NULL);
			tok = tok->next;
		}
		else if (handle_redirect(current_cmd, &tok))
			return (free_commands(cmd_list), NULL);
	}
	return (cmd_list);
}
