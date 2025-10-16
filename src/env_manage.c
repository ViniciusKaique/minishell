#include "../include/minishell.h"

/*
** @brief Duplica o array de variáveis de ambiente (envp).
*/
char	**dup_env(char **envp)
{
	char	**new_env;
	int		i;
	int		count;

	count = 0;
	while (envp[count])
		count++;
	new_env = ft_calloc(count + 1, sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			ft_free_matrix(new_env);
			return (NULL);
		}
		i++;
	}
	return (new_env);
}

/*
** @brief Busca o valor de uma variável de ambiente pelo nome.
*/
char	*get_env_value(char **envp, const char *name)
{
	int		i;
	int		name_len;
	char	*equal_sign;

	i = 0;
	name_len = ft_strlen(name);
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], name, name_len) == 0)
		{
			equal_sign = ft_strchr(envp[i], '=');
			if (equal_sign && (envp[i] + name_len) == equal_sign)
				return (ft_strdup(equal_sign + 1));
		}
		i++;
	}
	return (NULL);
}

/*
** @brief Adiciona ou atualiza uma variável de ambiente.
*/
int	set_env_value(char ***envp, const char *name, const char *value)
{
	int		i;
	int		count;
	char	*new_entry;
	char	**new_env;
	char	*temp;

	temp = ft_strjoin(name, "=");
	new_entry = ft_strjoin(temp, value);
	free(temp);
	i = -1;
	while ((*envp)[++i])
	{
		if (ft_strncmp((*envp)[i], name, ft_strlen(name)) == 0 && (*envp)[i][ft_strlen(name)] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = new_entry;
			return (0);
		}
	}
	count = i;
	new_env = ft_calloc(count + 2, sizeof(char *));
	i = -1;
	while (++i < count)
		new_env[i] = (*envp)[i];
	new_env[count] = new_entry;
	free(*envp);
	*envp = new_env;
	return (0);
}

/*
** @brief Implementa o comando built-in 'export'.
*/
int	builtin_export(char **argv, char ***envp)
{
	char	*name;
	char	*value;
	char	*equal_sign;
	int		i;

	if (!argv[1])
	{
		i = -1;
		while ((*envp)[++i])
			printf("declare -x %s\n", (*envp)[i]);
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		equal_sign = ft_strchr(argv[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			name = argv[i];
			value = equal_sign + 1;
			set_env_value(envp, name, value);
			*equal_sign = '=';
		}
		i++;
	}
	return (0);
}

/*
** @brief Implementa o comando built-in 'unset'.
*/
int	builtin_unset(char **argv, char ***envp)
{
	int		i;
	int		j;
	int		k;
	int		count;
	char	**new_env;
	int		len;

	count = 0;
	while ((*envp)[count])
		count++;
	new_env = ft_calloc(count + 1, sizeof(char *));
	i = -1;
	k = 0;
	while ((*envp)[++i])
	{
		j = 1;
		len = 0;
		while (argv[j] && ft_strncmp((*envp)[i], argv[j], ft_strlen(argv[j])) != 0)
			j++;
		if (!argv[j])
			new_env[k++] = ft_strdup((*envp)[i]);
	}
	ft_free_matrix(*envp);
	*envp = new_env;
	return (0);
}