/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:36:11 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/30 18:36:23 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static t_command	*handle_pipe(t_token **tok_ptr, t_command *current_cmd,
		t_command **cmd_list)
{
	t_token	*tok;

	tok = *tok_ptr;
	if (!tok->next || tok->next->type == T_PIPE || current_cmd->args == NULL)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		return (free_commands(*cmd_list), NULL);
	}
	current_cmd = create_and_append_cmd(cmd_list);
	if (!current_cmd)
		return (free_commands(*cmd_list), NULL);
	*tok_ptr = tok->next;
	return (current_cmd);
}

static t_command	*parse_loop(t_token *tok, t_command *cmd_list)
{
	t_command	*current_cmd;

	current_cmd = create_and_append_cmd(&cmd_list);
	if (!current_cmd)
		return (NULL);
	while (tok)
	{
		if (tok->type == T_WORD)
		{
			if (add_arg_to_cmd(current_cmd, tok->value, tok->has_quotes))
				return (free_commands(cmd_list), NULL);
			tok = tok->next;
		}
		else if (tok->type == T_PIPE)
		{
			current_cmd = handle_pipe(&tok, current_cmd, &cmd_list);
			if (!current_cmd)
				return (NULL);
		}
		else if (handle_redirect(current_cmd, &tok))
			return (free_commands(cmd_list), NULL);
	}
	return (cmd_list);
}

t_command	*parse(t_token *tokens)
{
	t_command	*cmd_list;

	cmd_list = NULL;
	return (parse_loop(tokens, cmd_list));
}
