/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 21:53:16 by vinpache          #+#    #+#             */
/*   Updated: 2025/11/09 21:53:30 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	heredoc_read_loop(int fd_write, const char *delimiter)
{
	char	*line;

	while (g_signal_received != 130)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_signal_received != 130)
			{
				ft_putstr_fd("minishell: warning: here-document at line 1 ", 2);
				ft_putendl_fd("delimited by end-of-file (wanted `EOF')", 2);
				return (1);
			}
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			return (0);
		}
		ft_putendl_fd(line, fd_write);
		free(line);
	}
	return (0);
}

static int	heredoc_open_and_dup(const char *tmpfile)
{
	int	fd_read;

	fd_read = open(tmpfile, O_RDONLY);
	if (fd_read < 0)
		return (perror("heredoc open"), 1);
	unlink(tmpfile);
	if (dup2(fd_read, STDIN_FILENO) < 0)
		return (perror("dup2"), close(fd_read), 1);
	close(fd_read);
	return (0);
}

int	handle_heredoc(char *delimiter)
{
	int			fd_write;
	const char	*tmpfile_name;
	int			eof_status;
	void		(*prev_sigint)(int);

	tmpfile_name = HEREDOC_TMP_FILE;
	fd_write = open(tmpfile_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd_write == -1)
		return (perror("open heredoc write"), 1);
	prev_sigint = signal(SIGINT, handle_sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
	g_signal_received = 0;
	eof_status = heredoc_read_loop(fd_write, delimiter);
	close(fd_write);
	signal(SIGINT, prev_sigint);
	if (g_signal_received == 130 || eof_status == 1)
	{
		unlink(tmpfile_name);
		return (1);
	}
	return (heredoc_open_and_dup(tmpfile_name));
}
