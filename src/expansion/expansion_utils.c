/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 15:22:05 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/30 17:41:20 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

void	handle_quotes(char c, char *quote_char)
{
	if (*quote_char == 0 && (c == '\'' || c == '"'))
		*quote_char = c;
	else if (*quote_char != 0 && c == *quote_char)
		*quote_char = 0;
}

static char	*extract_var_name(char *str, int *i)
{
	int		start;
	char	*name;

	(*i)++;
	if (str[*i] == '?')
	{
		name = ft_strdup("?");
		(*i)++;
	}
	else
	{
		start = *i;
		while (is_valid_var_char(str[*i]))
			(*i)++;
		name = ft_substr(str, start, *i - start);
	}
	return (name);
}

int	handle_variable(char *str, int i, char **new_str, t_env *env)
{
	char	*name;
	char	*value_from_env;
	char	*value_to_join;

	name = extract_var_name(str, &i);
	value_from_env = get_env_val(env, name);
	if (value_from_env == NULL)
		value_to_join = ft_strdup("");
	else
		value_to_join = ft_strdup(value_from_env);
	free(name);
	*new_str = ft_strjoin_free(*new_str, value_to_join);
	free(value_to_join);
	return (i);
}

void	append_char_to_str(char **s, char c)
{
	char	c_str[2];

	c_str[0] = c;
	c_str[1] = '\0';
	*s = ft_strjoin_free(*s, c_str);
}
