/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 19:02:00 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/27 18:16:17 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	prompt_loop(t_env **env)
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
				execute_commands(commands, env);
				free_commands(commands);
			}
			free_tokens(tokens);
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;

	(void)argc;
	(void)argv;
	env = init_env(envp);
	if (!env)
	{
		ft_putendl_fd("Erro ao inicializar ambiente.", 2);
		return (1);
	}
	prompt_loop(&env);
	free_env(env);
	return (0);
}
