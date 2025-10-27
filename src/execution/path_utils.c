/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:47:34 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/23 14:02:14 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *build_path(char *dir, char *cmd)
{
    char *tmp;
    char *path;

    tmp = ft_strjoin(dir, "/");  // concatena diretório com "/"
    if (!tmp)                    // checa falha de alocação
        return (NULL);
    path = ft_strjoin(tmp, cmd); // concatena "/"+cmd -> caminho completo
    free(tmp);                   // libera intermediário
    return(path);
}

char *find_command_path(char *cmd)
{
    char **dirs;
    char *path;
    int   i;

    dirs = ft_split(getenv("PATH"), ':'); // divide PATH por ‘:’
    if (!dirs)                            // se split falhar (ou PATH for NULL dentro do split), paramos
        return (NULL);
    i = -1;
    while (dirs[++i])                     // percorre cada diretório do PATH
    {
        path = build_path(dirs[i], cmd);  // cria "dir/ cmd"
        if (access(path, X_OK) == 0)      // existe e é executável?
            return (free_split(dirs), path); // libera dirs e retorna o caminho encontrado
        free(path);                       // não era executável -> libera e segue
    }
    free_split(dirs);                     // nada encontrado -> libera dirs
    return (NULL);                        // informa falha
}