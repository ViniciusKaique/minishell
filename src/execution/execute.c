/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 21:26:45 by vinpache          #+#    #+#             */
/*   Updated: 2025/11/09 21:49:17 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	save_stdio(int *stdin_backup, int *stdout_backup)
{
	*stdin_backup = dup(STDIN_FILENO);
	*stdout_backup = dup(STDOUT_FILENO);
}

static void	restore_stdio(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

static int	exec_single_builtin(t_command *cmds, t_env **env)
{
	int	exit_code;
	int	stdin_backup;
	int	stdout_backup;

	save_stdio(&stdin_backup, &stdout_backup);
	if (apply_redirects(cmds->redirects))
	{
		if (g_signal_received == 130)
			exit_code = 130;
		else
			exit_code = 1;
	}
	else if (cmds->args && cmds->args[0])
		exit_code = exec_builtin(cmds->args, env);
	else
		exit_code = 0;
	restore_stdio(stdin_backup, stdout_backup);
	return (exit_code);
}

int	execute_commands(t_command *cmds, t_env **env)
{
	if (!cmds)
		return (0);
	if (!cmds->args || !cmds->args[0])
	{
		if (cmds->redirects)
			return (exec_single_builtin(cmds, env));
		return (0);
	}
	if (cmds->next == NULL && is_builtin(cmds->args[0]))
		return (exec_single_builtin(cmds, env));
	else
		return (exec_pipeline(cmds, env));
}
