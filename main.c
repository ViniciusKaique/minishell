/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* main.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/10/22 16:36:21 by vinpache          #+#    #+#             */
/* Updated: 2025/10/26 16:25:00 by vinpache         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Função auxiliar para imprimir a estrutura de comandos (para teste).
 */
static void	print_commands(t_command *cmd)
{
	int			i;
	t_redirect	*redir;

	printf("--- Estrutura de Comandos Gerada ---\n");
	while (cmd)
	{
		i = 0;
		if (cmd->args)
		{
			printf("  Args: ");
			while (cmd->args[i])
			{
				printf("'%s' ", cmd->args[i]);
				i++;
			}
			printf("\n");
		}
		else
			printf("  Args: (null)\n");
		redir = cmd->redirects;
		printf("  Redirects: ");
		if (!redir)
			printf("(null)");
		while (redir)
		{
			printf("[type: %d, file: '%s'] ", redir->type, redir->file);
			redir = redir->next;
		}
		printf("\n------------------------------------\n");
		if (cmd->next)
			printf("                | (PIPE)\n");
		cmd = cmd->next;
	}
}

/**
 * @brief Loop principal do prompt do Minishell.
 */
static void	prompt_loop(void)
{
	char		*input;
	t_token		*tokens;
	t_command	*commands;

	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		if (*input)
		{
			add_history(input);
			tokens = tokenize(input);
			commands = parse(tokens);
			if (commands)
			{
				print_commands(commands);
				// AQUI ENTRARÁ O SEU EXECUTOR
				free_commands(commands);
			}
			free_tokens(tokens);
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;

	// Futuramente: inicializar sinais e envp aqui
	prompt_loop();
	return (0);
}