/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 19:15:53 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/29 19:16:04 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_single_builtin(t_command *cmds, t_env **env)
{
	int	exit_code;
	int	stdin_backup;
	int	stdout_backup;

	save_stdio(&stdin_backup, &stdout_backup);
	if (apply_redirects(cmds->redirects))
		exit_code = 1;
	else
		exit_code = exec_builtin(cmds->args, env);
	restore_stdio(stdin_backup, stdout_backup);
	return (exit_code);
}

int	execute_commands(t_command *cmds, t_env **env)
{
	if (!cmds)
		return (0);
	if (cmds->next == NULL && is_builtin(cmds->args[0]))
		return (exec_single_builtin(cmds, env));
	else
		return (exec_pipeline(cmds, env));
}
