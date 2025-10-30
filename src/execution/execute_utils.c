/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 20:47:17 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/30 20:47:23 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	*get_command_path(char **args)
{
	char	*path;

	if (!args || !args[0] || !*(args[0]))
	{
		ft_putendl_fd("minishell: : command not found", 2);
		exit(127);
	}
	if (ft_strchr(args[0], '/'))
		path = ft_strdup(args[0]);
	else
		path = find_command_path(args[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	return (path);
}

void	run_external_command(char **args, t_env *env)
{
	char	*path;
	char	**envp;

	path = get_command_path(args);
	handle_path_pre_exec_errors(path, args);
	envp = env_to_array(env);
	signal(SIGPIPE, SIG_DFL);
	execve(path, args, envp);
	perror(args[0]);
	if (errno == EACCES)
		exit(126);
	else
		exit(127);
	free(path);
	ft_free_matrix(envp);
}
