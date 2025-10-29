/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 19:21:41 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/29 19:22:00 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_redir_in(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (perror(file), 1);
	if (dup2(fd, STDIN_FILENO) < 0)
		return (perror("dup2"), close(fd), 1);
	close(fd);
	return (0);
}

static int	handle_redir_out(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(file), 1);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (perror("dup2"), close(fd), 1);
	close(fd);
	return (0);
}

static int	handle_redir_append(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror(file), 1);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (perror("dup2"), close(fd), 1);
	close(fd);
	return (0);
}

int	apply_redirects(t_redirect *redir)
{
	int	status;

	status = 0;
	while (redir && status == 0)
	{
		if (redir->type == R_IN)
			status = handle_redir_in(redir->file);
		else if (redir->type == R_OUT_TRUNC)
			status = handle_redir_out(redir->file);
		else if (redir->type == R_OUT_APPEND)
			status = handle_redir_append(redir->file);
		else if (redir->type == R_HEREDOC)
			status = handle_heredoc(redir->file);
		redir = redir->next;
	}
	return (status);
}
