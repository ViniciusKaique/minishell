/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 14:04:53 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/30 15:01:47 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*find_env(t_env *env, const char *name)
{
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

static int	update_existing_env(t_env *node, const char *value)
{
	char	*v_dup;

	if (value)
		v_dup = ft_strdup(value);
	else
		v_dup = NULL;
	if (value && v_dup == NULL)
		return (1);
	if (node->value)
		free(node->value);
	node->value = v_dup;
	return (0);
}

static int	create_new_env(t_env **env, const char *name, const char *value)
{
	char	*n_dup;
	char	*v_dup;

	n_dup = ft_strdup(name);
	if (!n_dup)
		return (1);
	if (value)
		v_dup = ft_strdup(value);
	else
		v_dup = NULL;
	if (value && v_dup == NULL)
	{
		free(n_dup);
		return (1);
	}
	add_env_back(env, new_env(n_dup, v_dup));
	return (0);
}

int	set_env_kv(t_env **env, const char *name, const char *value)
{
	t_env	*node;

	node = find_env(*env, name);
	if (node)
		return (update_existing_env(node, value));
	else
		return (create_new_env(env, name, value));
}

char	*get_env_val(t_env *env, const char *name)
{
	t_env	*n;

	n = find_env(env, name);
	if (n)
		return (n->value);
	return (NULL);
}
