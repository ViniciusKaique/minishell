/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 00:00:00 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/29 14:47:09 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

void	handle_sigint_prompt(int sig)
{
	(void)sig;
	g_signal_received = 130; // Define o status de saída para ctrl-C
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_signal_received = 130; // Define o status
	// Não precisa de write, rl_replace_line, etc.
	// Apenas fecha o STDIN para interromper o readline() do heredoc.
	close(STDIN_FILENO);
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint_prompt); // <-- Usa o novo handler padrão
	signal(SIGQUIT, SIG_IGN);
 }
