/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:28:01 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/27 17:28:03 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implementa o comando `pwd`
 * 
 * Mostra o diretório atual de trabalho.
 * Usa getcwd() para recuperar o caminho absoluto.
 */
int	builtin_pwd(void)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putendl_fd(cwd, 1); // imprime no stdout
		return (0);
	}
	perror("pwd");             // se falhar, exibe erro do sistema
	return (1);
}
