/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 21:56:19 by vinpache          #+#    #+#             */
/*   Updated: 2025/11/09 21:56:59 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_input(char *input, t_env **env);

static void	update_status_from_signal(t_env **env)
{
	char	*status_str;

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
}

static void	prompt_loop(t_env **env)
{
	char	*input;

	while (1)
	{
		setup_signals();
		update_status_from_signal(env);
		input = readline("minishell$ ");
		if (!input)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		if (*input)
			handle_input(input, env);
		free(input);
	}
}

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
