/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrito-g <kbrito-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:34:03 by vinpache          #+#    #+#             */
/*   Updated: 2025/11/05 16:27:46 by kbrito-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->args_has_quotes = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	add_command_back(t_command **list, t_command *new)
{
	t_command	*tmp;

	if (!list || !new)
		return ;
	if (!*list)
	{
		*list = new;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

static int	realloc_arg_arrays(t_command *cmd, char ***new_args,
		int **new_quotes)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (cmd->args && cmd->args[i])
		i++;
	count = i;
	*new_args = ft_calloc(count + 2, sizeof(char *));
	if (!*new_args)
		return (-1);
	*new_quotes = ft_calloc(count + 2, sizeof(int));
	if (!*new_quotes)
	{
		free(*new_args);
		return (-1);
	}
	i = 0;
	while (i < count)
	{
		(*new_args)[i] = cmd->args[i];
		(*new_quotes)[i] = cmd->args_has_quotes[i];
		i++;
	}
	return (count);
}

int	add_arg_to_cmd(t_command *cmd, char *arg_val, int has_quotes)
{
	char	**new_args;
	int		*new_quotes;
	char	*arg_copy;
	int		count;

	count = realloc_arg_arrays(cmd, &new_args, &new_quotes);
	if (count == -1)
		return (1);
	arg_copy = ft_strdup(arg_val);
	if (!arg_copy)
	{
		free(new_args);
		free(new_quotes);
		return (1);
	}
	new_args[count] = arg_copy;
	new_quotes[count] = has_quotes;
	if (cmd->args)
		free(cmd->args);
	if (cmd->args_has_quotes)
		free(cmd->args_has_quotes);
	cmd->args = new_args;
	cmd->args_has_quotes = new_quotes;
	return (0);
}

t_command	*create_and_append_cmd(t_command **cmd_list)
{
	t_command	*new_cmd;

	new_cmd = new_command();
	if (!new_cmd)
		return (NULL);
	add_command_back(cmd_list, new_cmd);
	return (new_cmd);
}
