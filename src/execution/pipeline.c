/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 19:47:14 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/30 20:39:52 by vinpache         ###   ########.fr       */
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

static void	exec_child(t_command *cmd, t_env **env, int in_fd, int pipefd[2])
{
	int	builtin_exit_code;
	int	out_fd;

	if (cmd->next)
		out_fd = pipefd[1];
	else
		out_fd = STDOUT_FILENO;
	setup_child_io_and_signals(in_fd, out_fd);
	if (cmd->next)
		close(pipefd[0]);
	if (apply_redirects(cmd->redirects))
		exit(1);
	if (is_builtin(cmd->args[0]))
	{
		builtin_exit_code = exec_builtin(cmd->args, env);
		exit(builtin_exit_code);
	}
	run_external_command(cmd->args, *env);
}

static pid_t	fork_pipeline_loop(t_command *cmds, t_env **env, int *in_fd)
{
	pid_t	pid;
	int		pipefd[2];

	pid = -1;
	while (cmds)
	{
		if (cmds->next && pipe(pipefd) == -1)
			return (perror("pipe"), -1);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), -1);
		else if (pid == 0)
			exec_child(cmds, env, *in_fd, pipefd);
		if (*in_fd != STDIN_FILENO)
			close(*in_fd);
		if (cmds->next)
		{
			close(pipefd[1]);
			*in_fd = pipefd[0];
		}
		cmds = cmds->next;
	}
	return (pid);
}

static int	wait_for_pipeline(pid_t last_pid)
{
	int		status;
	int		exit_code;
	pid_t	wait_pid;

	wait_pid = 0;
	status = 0;
	exit_code = 0;
	while (wait_pid != -1)
	{
		wait_pid = waitpid(-1, &status, 0);
		if (wait_pid <= 0)
			continue ;
		if (wait_pid == last_pid)
			exit_code = get_pipeline_exit_status(status);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGPIPE)
			ft_putendl_fd(" Broken pipe", 2);
		else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			ft_putstr_fd("\n", 2);
		else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", 2);
	}
	setup_signals();
	return (exit_code);
}

int	exec_pipeline(t_command *cmds, t_env **env)
{
	int		in_fd;
	pid_t	last_pid;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	in_fd = STDIN_FILENO;
	last_pid = fork_pipeline_loop(cmds, env, &in_fd);
	if (last_pid == -1)
		return (1);
	return (wait_for_pipeline(last_pid));
}
