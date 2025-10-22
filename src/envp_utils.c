#include <stdlib.h>
#include "minishell.h"

t_env	*new_env(char *name, char *value)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->name = name;     // futuramente: ft_strdup(name)
	env->value = value;   // futuramente: ft_strdup(value)
	env->next = NULL;
	return (env);
}

void	add_env_back(t_env **list, t_env *new)
{
	t_env	*tmp;

	if (!list || !new)
		return ;
	if (!*list)
	{
		*list = new;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}