/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:57:33 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/30 17:57:50 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_tilde(t_env *env, const char *arg)
{
	const char	*home;
	char		*tmp;
	char		*out;

	home = get_env_val(env, "HOME");
	if (!home)
		return (NULL);
	if (arg[1] == '\0')
		return (ft_strdup(home));
	if (arg[1] == '/')
	{
		tmp = ft_strjoin(home, "/");
		if (!tmp)
			return (NULL);
		out = ft_strjoin(tmp, arg + 2);
		free(tmp);
		return (out);
	}
	return (ft_strdup(arg));
}

static char	*get_path_from_env(t_env *env, const char *env_var)
{
	char	*val;

	val = get_env_val(env, env_var);
	if (!val)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd((char *)env_var, 2);
		ft_putendl_fd(" not set", 2);
		return (NULL);
	}
	return (ft_strdup(val));
}

static char	*get_target_path(char **args, t_env *env)
{
	char	*path;

	if (!args[1] || !args[1][0])
		path = get_path_from_env(env, "HOME");
	else if (ft_strcmp(args[1], "-") == 0)
		path = get_path_from_env(env, "OLDPWD");
	else if (args[1][0] == '~')
	{
		path = expand_tilde(env, args[1]);
		if (!path)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2), NULL);
	}
	else
		path = ft_strdup(args[1]);
	return (path);
}

static void	handle_post_cd_actions(t_env **env, char *oldpwd, char **args)
{
	char	newpwd[4096];
	char	*arg1_cmp;

	if (getcwd(newpwd, sizeof(newpwd)) != NULL)
	{
		if (args[1])
			arg1_cmp = args[1];
		else
			arg1_cmp = "";
		if (ft_strcmp(arg1_cmp, "-") == 0)
			ft_putendl_fd(newpwd, 1);
		if (oldpwd[0] != '\0')
			set_env_kv(env, "OLDPWD", oldpwd);
		set_env_kv(env, "PWD", newpwd);
	}
}

int	builtin_cd(char **args, t_env **env)
{
	char	*path;
	char	oldpwd[4096];

	if (args[1] && args[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		return (1);
	}
	if (getcwd(oldpwd, sizeof(oldpwd)) == NULL)
		oldpwd[0] = '\0';
	path = get_target_path(args, *env);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		perror(NULL);
		free(path);
		return (1);
	}
	handle_post_cd_actions(env, oldpwd, args);
	free(path);
	return (0);
}
