#include "../include/minishell.h"

// As 5 funções de main.c
volatile sig_atomic_t	g_sig = 0;

void	sigint_handler(int sig)
{
	g_sig = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	init_minishell(void)
{
	setup_signals();
}

void	prompt_loop(char **envp, struct termios *original_termios)
{
	char	*line;
	char	**tokens;
	t_cmd	*cmds;
	int		last_status;

	last_status = 0;
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		tokens = split_tokens(line);
		if (tokens && build_commands(tokens, &cmds, envp, last_status) == 0)
		{
			execute_pipeline(cmds, &envp, &last_status, original_termios);
		}
		free_tokens(tokens);
		// free_cmds(&cmds);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char			**env_copy;
	struct termios	original_termios;
	struct termios	new_termios;

	(void)argc;
	(void)argv;
	tcgetattr(STDIN_FILENO, &original_termios);
	new_termios = original_termios;
	new_termios.c_lflag &= ~ECHOCTL; // Desativa o eco de ^C
	tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
	
	env_copy = dup_env(envp);
	init_minishell();
	prompt_loop(env_copy, &original_termios); // Passa as config. originais
	
	// Restaura o terminal na saída normal
	tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
	ft_free_matrix(env_copy);
	return (0);
}