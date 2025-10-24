/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 14:06:32 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/23 14:14:23 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec(char **args, char **envp)
{
	pid_t pid;
	int status;
	char *path;

	path = find_command_path(args[0]); // busca executável (ex: /bin/ls)
	if (!path)
	{
		printf("Comando não encontrado: %s\n", args[0]);
		return ;
	}

	pid = fork(); // cria o processo filho
	if (pid == 0)
	{
		// código do filho
		execve(path, args, envp); // substitui o processo
		perror("execve"); // se falhar, mostra erro
		exit(127); // código de erro padrão de comando inexistente
	}
	else if (pid > 0)
	{
		// código do pai
		waitpid(pid, &status, 0); // espera o filho terminar
		printf("Comando terminou com status: %d\n", WEXITSTATUS(status));
	}
	else
		perror("fork");

	free(path);
}