/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 18:02:40 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/27 18:02:40 by vinpache         ###   ########.fr       */
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

int	builtin_export(char **args, t_env **env)
{
	int		i;
	char	*eq;

	if (!args[1])
		return (print_export_list(*env), 0);
	i = 1;
	while (args[i])
	{
		eq = ft_strchr(args[i], '=');
		if (eq)
		{
			*eq = '\0';
			if (is_valid_name(args[i]))
				set_env_kv(env, args[i], eq + 1);
			else
			{
				ft_putstr_fd("minishell: export: `", 2);
				ft_putstr_fd(args[i], 2);
				ft_putendl_fd("': not a valid identifier", 2);
			}
			*eq = '=';
		}
		else if (is_valid_name(args[i]))
			set_env_kv(env, args[i], NULL);
		else
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
		}
		i++;
	}
	return (0);
}
