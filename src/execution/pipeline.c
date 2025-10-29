/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 19:12:10 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/29 19:12:11 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_child_io_and_signals(int in_fd, int out_fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
}

static void	run_external_command(char **args, t_env *env)
{
	char	*path;
	char	**envp;

	path = find_command_path(args[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(args[0], 2);
		exit(127);
	}
	envp = env_to_array(env);
	execve(path, args, envp);
	perror("execve");
	free(path);
	ft_free_matrix(envp);
	exit(126);
}

static void	exec_child(t_command *cmd, t_env **env, int in_fd, int pipefd[2])
{
	int	builtin_exit_code;
	int	out_fd;

	if (cmd->next)
		out_fd = pipefd[1];
	else
		out_fd = STDOUT_FILENO;
	setup_child_io_and_signals(in_fd, out_fd);
	if (apply_redirects(cmd->redirects))
		exit(1);
	if (is_builtin(cmd->args[0]))
	{
		builtin_exit_code = exec_builtin(cmd->args, env);
		exit(builtin_exit_code);
	}
	run_external_command(cmd->args, *env);
}

static void	wait_and_manage_fds(pid_t pid, int *status, t_pipe_data *pipe_data,
		int has_next)
{
	waitpid(pid, status, 0);
	if (WIFSIGNALED(*status))
	{
		if (WTERMSIG(*status) == SIGINT)
			ft_putstr_fd("\n", 2);
		else if (WTERMSIG(*status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", 2);
	}
	if (*(pipe_data->in_fd) != STDIN_FILENO)
		close(*(pipe_data->in_fd));
	if (has_next)
	{
		close(pipe_data->pipefd[1]);
		*(pipe_data->in_fd) = pipe_data->pipefd[0];
	}
}

int	exec_pipeline(t_command *cmds, t_env **env)
{
	t_pipe_data	pipe_data;
	int			in_fd;
	pid_t		pid;
	int			status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	in_fd = STDIN_FILENO;
	pipe_data.in_fd = &in_fd;
	status = 0;
	while (cmds)
	{
		if (cmds->next && pipe(pipe_data.pipefd) == -1)
			return (perror("pipe"), 1);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);
		else if (pid == 0)
			exec_child(cmds, env, in_fd, pipe_data.pipefd);
		wait_and_manage_fds(pid, &status, &pipe_data, (cmds->next != NULL));
		cmds = cmds->next;
	}
	setup_signals();
	return (get_pipeline_exit_status(status));
}
