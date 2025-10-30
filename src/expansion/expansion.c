/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:49:12 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/30 18:52:56 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_expansion_char(char *str, char **new_str, t_env *env,
		char *quote_char)
{
	int		i;
	char	next_char;

	i = 0;
	if ((*quote_char == 0 && (str[i] == '\'' || str[i] == '"'))
		|| (*quote_char != 0 && str[i] == *quote_char))
	{
		handle_quotes(str[i], quote_char);
		i++;
	}
	else if (str[i] == '$' && *quote_char != '\'')
	{
		next_char = str[i + 1];
		if (is_valid_var_char(next_char) || next_char == '?')
			i = handle_variable(str, i, new_str, env);
		else
			append_char_to_str(new_str, str[i++]);
	}
	else
		append_char_to_str(new_str, str[i++]);
	return (i);
}

char	*expand_and_remove_quotes(char *str, t_env *env)
{
	char	*new_str;
	char	quote_char;
	int		i;

	new_str = ft_strdup("");
	quote_char = 0;
	i = 0;
	while (str[i])
	{
		i += process_expansion_char(str + i, &new_str, env, &quote_char);
	}
	return (new_str);
}

static void	split_and_join_token(char *str, char **final_str)
{
	char	**split_args;
	int		j;

	split_args = ft_split(str, ' ');
	if (!split_args)
		return ;
	j = 0;
	while (split_args[j])
	{
		*final_str = ft_strjoin_free(*final_str, split_args[j]);
		*final_str = ft_strjoin_free(*final_str, "\x1F");
		j++;
	}
	ft_free_matrix(split_args);
}

static void	expand_args(t_command *cmd, t_env *env)
{
	char	*final_string;
	char	*expanded_str;
	int		i;

	final_string = ft_strdup("");
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		expanded_str = expand_and_remove_quotes(cmd->args[i], env);
		if (cmd->args_has_quotes[i] == 1 || !*expanded_str)
		{
			final_string = ft_strjoin_free(final_string, expanded_str);
			final_string = ft_strjoin_free(final_string, "\x1F");
		}
		else
			split_and_join_token(expanded_str, &final_string);
		free(expanded_str);
		i++;
	}
	ft_free_matrix(cmd->args);
	free(cmd->args_has_quotes);
	cmd->args = ft_split(final_string, '\x1F');
	cmd->args_has_quotes = NULL;
	free(final_string);
}

void	expand_commands(t_command *cmds, t_env *env)
{
	t_command	*cmd_ptr;

	cmd_ptr = cmds;
	while (cmd_ptr)
	{
		expand_args(cmd_ptr, env);
		expand_redirects(cmd_ptr, env);
		cmd_ptr = cmd_ptr->next;
	}
}
