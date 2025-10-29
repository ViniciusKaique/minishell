/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 19:18:12 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/29 19:51:28 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* heredoc.c                                          :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: ParceiroProg <ParceiroProg@42.fr>        +#+  +:+       +#+        */
/* +#+           */
/* Created: 2025/10/29 19:15:00 by ParceiroProg     #+#    #+#             */
/* Updated: 2025/10/29 19:15:00 by ParceiroProg     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_read_loop(int fd_write, const char *delimiter)
{
	char	*line;

	signal(SIGINT, handle_sigint_heredoc);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		ft_putendl_fd(line, fd_write);
		free(line);
	}
	signal(SIGINT, handle_sigint_prompt);
}

static int	heredoc_open_and_dup(const char *tmpfile)
{
	int	fd_read;

	fd_read = open(tmpfile, O_RDONLY);
	unlink(tmpfile);
	if (fd_read < 0)
		return (perror("heredoc open"), 1);
	if (dup2(fd_read, STDIN_FILENO) < 0)
		return (perror("dup2"), close(fd_read), 1);
	close(fd_read);
	return (0);
}

int	handle_heredoc(char *delimiter)
{
	char	tmpfile[23];
	int		fd_write;

	fd_write = open(tmpfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd_write == -1)
		return (perror("open (heredoc)"), 1);
	heredoc_read_loop(fd_write, delimiter);
	close(fd_write);
	return (heredoc_open_and_dup(tmpfile));
}
