/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 18:36:33 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/29 19:02:14 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	save_stdio(int *stdin_backup, int *stdout_backup)
{
	*stdin_backup = dup(STDIN_FILENO);
	*stdout_backup = dup(STDOUT_FILENO);
}

void	restore_stdio(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

static int	count_env_vars(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->value)
			count++;
		env = env->next;
	}
	return (count);
}

int	get_pipeline_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (status);
}

char	**env_to_array(t_env *env)
{
	char	**array;
	char	*joined;
	int		i;
	int		count;

	count = count_env_vars(env);
	array = ft_calloc(count + 1, sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->value)
		{
			joined = ft_strjoin(env->name, "=");
			array[i] = ft_strjoin_free(joined, env->value);
			i++;
		}
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}
