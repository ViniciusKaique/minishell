/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 18:13:27 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/27 18:13:27 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(char **args, t_env **env)
{
	char	*path;
	char	oldpwd[4096];
	char	newpwd[4096];
	t_env	*home_env;

	if (getcwd(oldpwd, sizeof(oldpwd)) == NULL)
		oldpwd[0] = '\0';

	if (args[1] == NULL)
	{
		home_env = find_env(*env, "HOME");
		if (!home_env || !home_env->value)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (1);
		}
		path = home_env->value;
	}
	else
		path = args[1];

	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	if (getcwd(newpwd, sizeof(newpwd)) != NULL)
	{
		if (oldpwd[0] != '\0')
			set_env_kv(env, "OLDPWD", oldpwd);
		set_env_kv(env, "PWD", newpwd);
	}
	return (0);
}
