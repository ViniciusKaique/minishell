/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 18:02:29 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/27 18:02:30 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_env *env)
{
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd(env->name, 1);
			ft_putchar_fd('=', 1);
			ft_putendl_fd(env->value, 1);
		}
		env = env->next;
	}
	return (0);
}
