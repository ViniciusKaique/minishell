/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 18:02:58 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/27 18:02:59 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(char **args, t_env **env)
{
	t_env	*cur;
	t_env	*prev;
	int		i;

	i = 1;
	while (args[i])
	{
		if (!is_valid_name(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			i++;
			continue;
		}
		cur = *env;
		prev = NULL;
		while (cur)
		{
			if (ft_strcmp(cur->name, args[i]) == 0)
			{
				if (prev)
					prev->next = cur->next;
				else
					*env = cur->next;
				free(cur->name);
				free(cur->value);
				free(cur);
				break;
			}
			prev = cur;
			cur = cur->next;
		}
		i++;
	}
	return (0);
}
