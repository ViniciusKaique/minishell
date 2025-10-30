/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:47:34 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/30 14:00:57 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (path);
}

char	*find_command_path(char *cmd)
{
	char	**dirs;
	char	*path;
	int		i;

	dirs = ft_split(getenv("PATH"), ':');
	if (!dirs)
		return (NULL);
	i = -1;
	while (dirs[++i])
	{
		path = build_path(dirs[i], cmd);
		if (access(path, X_OK) == 0)
			return (free_split(dirs), path);
		free(path);
	}
	free_split(dirs);
	return (NULL);
}
