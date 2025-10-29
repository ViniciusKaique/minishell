/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 16:34:55 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/29 16:34:56 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief O loop principal do shell.
 * * Este loop:
 * 1. Verifica se (ctrl-C) foi pressionado no prompt anterior (via g_signal_received).
 * 2. Lê a entrada do utilizador com readline.
 * 3. Tokeniza, faz o parsing e a expansão da entrada.
 * 4. Executa os comandos.
 * 5. Atualiza o status de saída $? com base no resultado da execução.
 */
static void	prompt_loop(t_env **env)
{
	char		*input;
	t_token		*tokens;
	t_command	*commands;
	char		*status_str;
	int			exit_code;

	while (1)
	{
		/* * Se o handler de sinal (do prompt) foi ativado,
		 * g_signal_received terá o valor 130.
		 */
		if (g_signal_received != 0)
		{
			status_str = ft_itoa(g_signal_received);
			if (status_str)
			{
				set_env_kv(env, "?", status_str); //
				free(status_str);
			}
			g_signal_received = 0; // Resetar a flag
		}

		input = readline("minishell$ ");
		if (!input)
		{
			ft_putendl_fd("exit", 1); //
			break ; // (ctrl-D)
		}
		if (*input)
		{
			add_history(input); //
			tokens = tokenize(input); //
			commands = parse(tokens); //
			if (commands)
			{
				/* 1. Expande variáveis (ex: $USER, $?) e remove aspas */
				expand_commands(commands, *env);
				
				/* 2. Executa os comandos e recebe o código de saída */
				exit_code = execute_commands(commands, env); //

				/* 3. Atualiza $? com o código de saída da execução */
				status_str = ft_itoa(exit_code); //
				if (status_str)
				{
					set_env_kv(env, "?", status_str); //
					free(status_str);
				}
				free_commands(commands); //
			}
			free_tokens(tokens); //
		}
		free(input); //
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;

	(void)argc;
	(void)argv;
	
	/* 1. Carregar o ambiente (PATH, HOME, etc.) */
	env = init_env(envp); //
	if (!env)
	{
		ft_putendl_fd("Erro ao inicializar ambiente.", 2); //
		return (1);
	}

	/* 2. Inicializar o status de saída $? como "0" */
	set_env_kv(&env, "?", "0"); //

	/* 3. Configurar os sinais para o prompt (ignorar \ e tratar C) */
	setup_signals(); //
	
	/* 4. Iniciar o loop principal */
	prompt_loop(&env); //
	
	/* 5. Limpar a memória do ambiente antes de sair */
	free_env(env); //
	return (0);
}
