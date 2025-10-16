#include "../include/minishell.h"

/*
** @brief Abre arquivos para redirecionamentos e configura os fds no comando.
*/
int	handle_redirections(t_cmd *cmd)
{
	// Lógica para iterar sobre cmd->redirs e usar open()
	// ... (código simplificado para o exemplo) ...
	if (cmd->redirs && cmd->redirs[0] && ft_strcmp(cmd->redirs[0], ">") == 0)
	{
		if (cmd->out_fd != -1)
			close(cmd->out_fd);
		cmd->out_fd = open(cmd->redirs[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (cmd->out_fd < 0)
		{
			perror(cmd->redirs[1]);
			return (-1);
		}
	}
	return (0);
}

/*
** @brief Configura os descritores de entrada/saída para um comando.
*/
int	setup_fds_for_cmd(t_cmd *cmd, int in_fd, int out_fd)
{
	if (cmd->in_fd != -1)
		in_fd = cmd->in_fd;
	if (in_fd != -1)
	{
		if (dup2(in_fd, STDIN_FILENO) < 0)
			return (-1);
		close(in_fd);
	}
	if (cmd->out_fd != -1)
		out_fd = cmd->out_fd;
	if (out_fd != -1)
	{
		if (dup2(out_fd, STDOUT_FILENO) < 0)
			return (-1);
		close(out_fd);
	}
	return (0);
}

/*
** @brief Procura e executa um comando externo.
*/
int	find_and_exec_command(t_cmd *cmd, char **envp)
{
	char	**paths;
	char	*path_val;
	char	*full_path;
	int		i;

	if (ft_strchr(cmd->argv[0], '/'))
		execve(cmd->argv[0], cmd->argv, envp);
	path_val = get_env_value(envp, "PATH");
	paths = ft_split(path_val, ':');
	free(path_val);
	i = -1;
	while (paths && paths[++i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin_free(full_path, cmd->argv[0]);
		if (access(full_path, X_OK) == 0)
			execve(full_path, cmd->argv, envp);
		free(full_path);
	}
	ft_free_matrix(paths);
	write(2, "minishell: command not found\n", 29);
	exit(127);
	return (-1);
}

/*
** @brief Ponto de entrada para a execução em um processo filho.
*/
int	execute_child(t_cmd *cmd, char **envp, int in_fd, int out_fd)
{
	char	*cmd_name;

	cmd_name = cmd->argv[0];
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (handle_redirections(cmd) != 0)
		exit(1);
	if (setup_fds_for_cmd(cmd, in_fd, out_fd) != 0)
		exit(1);
	if (ft_strcmp(cmd_name, "echo") == 0)
		exit(builtin_echo(cmd->argv));
	if (ft_strcmp(cmd_name, "pwd") == 0)
		exit(builtin_pwd());
	if (ft_strcmp(cmd_name, "env") == 0)
		exit(builtin_env(envp));
	find_and_exec_command(cmd, envp);
	exit(1);
}

/*
** @brief Gerencia a execução de um pipeline de um ou mais comandos.
*/
int	execute_pipeline(t_cmd *cmds, char ***envp, int *last_status,
		struct termios *termios)
{
	int pid;
	int pipe_fds[2];
	int in_fd;
	char *cmd_name;

	if (!cmds || !cmds->argv || !cmds->argv[0])
		return (0);
	cmd_name = cmds->argv[0];
	if (!cmds->next && (ft_strcmp(cmd_name, "cd") == 0 || ft_strcmp(cmd_name,
				"export") == 0 || ft_strcmp(cmd_name, "unset") == 0
			|| ft_strcmp(cmd_name, "exit") == 0))
	{
		if (ft_strcmp(cmd_name, "cd") == 0)
			*last_status = builtin_cd(cmds->argv, envp);
		// CORRIJA ESTA LINHA:
		if (ft_strcmp(cmd_name, "export") == 0)
			*last_status = builtin_export(cmds->argv, envp);
				// Era "last_statuos"
		if (ft_strcmp(cmd_name, "unset") == 0)
			*last_status = builtin_unset(cmds->argv, envp);
		if (ft_strcmp(cmd_name, "exit") == 0)
			builtin_exit(cmds->argv, *envp, termios);
		return (0);
	}
	// --- A LÓGICA DO PIPELINE COMEÇA AQUI, DENTRO DA FUNÇÃO ---
	in_fd = -1;
	while (cmds)
	{
		if (cmds->next)
			pipe(pipe_fds);
		pid = fork();
		if (pid == 0)
		{
			if (cmds->next)
			{
				close(pipe_fds[0]);
				execute_child(cmds, *envp, in_fd, pipe_fds[1]);
			}
			else
				execute_child(cmds, *envp, in_fd, -1);
		}
		if (in_fd != -1)
			close(in_fd);
		if (cmds->next)
		{
			close(pipe_fds[1]);
			in_fd = pipe_fds[0];
		}
		cmds = cmds->next;
	}
	waitpid(pid, last_status, 0);
	while (wait(NULL) > 0)
		;
	// --- ESTE RETURN GARANTE QUE A FUNÇÃO SEMPRE RETORNE UM VALOR ---
	return (0);
}