/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:14:06 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/30 17:14:20 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_input(char *input, t_env **env)
{
	t_token		*tokens;
	t_command	*commands;
	char		*status_str;
	int			exit_code;

	add_history(input);
	tokens = tokenize(input);
	commands = parse(tokens);
	if (commands)
	{
		expand_commands(commands, *env);
		exit_code = execute_commands(commands, env);
		status_str = ft_itoa(exit_code);
		if (status_str)
		{
			set_env_kv(env, "?", status_str);
			free(status_str);
		}
		free_commands(commands);
	}
	free_tokens(tokens);
}

static void	prompt_loop(t_env **env)
{
	char	*input;
	char	*status_str;

	while (1)
	{
		if (g_signal_received != 0)
		{
			status_str = ft_itoa(g_signal_received);
			if (status_str)
			{
				set_env_kv(env, "?", status_str);
				free(status_str);
			}
			g_signal_received = 0;
		}
		input = readline("minishell$ ");
		if (!input)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		if (*input)
			handle_input(input, env);
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
	set_env_kv(&env, "?", "0");
	setup_signals();
	prompt_loop(&env);
	free_env(env);
	return (0);
}
