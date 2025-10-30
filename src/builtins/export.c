/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:51:11 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/30 17:51:47 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_export_list(t_env *env)
{
	while (env)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(env->name, 1);
		if (env->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(env->value, 1);
			ft_putendl_fd("\"", 1);
		}
		else
			ft_putchar_fd('\n', 1);
		env = env->next;
	}
}

static void	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

static int	process_export_arg(char *arg, t_env **env)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		*eq = '\0';
		if (is_valid_name(arg))
			set_env_kv(env, arg, eq + 1);
		else
		{
			print_export_error(arg);
			*eq = '=';
			return (1);
		}
		*eq = '=';
	}
	else if (!is_valid_name(arg))
	{
		print_export_error(arg);
		return (1);
	}
	else
		set_env_kv(env, arg, NULL);
	return (0);
}

int	builtin_export(char **args, t_env **env)
{
	int	i;
	int	exit_status;

	if (!args[1])
		return (print_export_list(*env), 0);
	i = 1;
	exit_status = 0;
	while (args[i])
	{
		if (process_export_arg(args[i], env) != 0)
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
