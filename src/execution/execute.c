#include "minishell.h"

/**
 * @brief Executa um comando simples (sem pipe/redirecionamento).
 * 
 * Se for builtin, executa direto no processo atual.
 * Se for comando externo, faz fork + execve().
 */
void	execute_commands(t_command *cmds, t_env **env)
{
	t_command	*cmd;
	pid_t		pid;
	char		*path;
	char		**envp;
	int			status;

	cmd = cmds;
	while (cmd)
	{
		if (!cmd->args || !cmd->args[0])
		{
			cmd = cmd->next;
			continue;
		}

		// --- executa builtin direto ---
		if (is_builtin(cmd->args[0]))
		{
			exec_builtin(cmd->args, env);
			cmd = cmd->next;
			continue;
		}

		// --- comando externo ---
		path = find_command_path(cmd->args[0]);
		if (!path)
		{
			ft_putstr_fd("minishell: command not found: ", 2);
			ft_putendl_fd(cmd->args[0], 2);
			cmd = cmd->next;
			continue;
		}

		pid = fork();
		if (pid == 0)
		{
			envp = env_to_array(*env); // converte t_env para char**
			execve(path, cmd->args, envp);
			perror("execve");
			exit(127);
		}
		else if (pid > 0)
			waitpid(pid, &status, 0);
		else
			perror("fork");

		free(path);
		cmd = cmd->next;
	}
}

/**
 * @brief Converte a lista t_env em um array "NAME=VALUE" para o execve().
 */
char	**env_to_array(t_env *env)
{
	int		count;
	t_env	*tmp;
	char	**array;
	char	*joined;
	int		i;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
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
