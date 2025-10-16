#include "../include/minishell.h"

/*
** @brief Divide a linha de entrada em tokens, tratando aspas corretamente.
** NOTA: Toda a lógica está contida nesta função para respeitar a regra de
** 5 funções por arquivo, sem criar funções auxiliares 'static'.
*/
char	**split_tokens(const char *line)
{
	char	**tokens;
	int		i;
	int		j;
	int		count;
	int		start;
	char	quote;

	if (!line)
		return (NULL);
	// --- 1ª Passagem: Contar os tokens ---
	i = 0;
	count = 0;
	while (line[i])
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (line[i])
			count++;
		if (line[i] == '\'' || line[i] == '\"')
		{
			quote = line[i++];
			while (line[i] && line[i] != quote)
				i++;
			if (line[i])
				i++;
		}
		else
			while (line[i] && line[i] != ' ' && line[i] != '\t')
				i++;
	}
	// --- Alocação ---
	tokens = ft_calloc(count + 1, sizeof(char *));
	if (!tokens)
		return (NULL);
	// --- 2ª Passagem: Extrair os tokens ---
	i = 0;
	j = 0;
	while (line[i] && j < count)
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		start = i;
		if (line[i] == '\'' || line[i] == '\"')
		{
			quote = line[i++];
			while (line[i] && line[i] != quote)
				i++;
			if (line[i])
				i++;
		}
		else
			while (line[i] && line[i] != ' ' && line[i] != '\t')
				i++;
		tokens[j++] = ft_substr(line, start, i - start);
	}
	return (tokens);
}

/*
** @brief Constrói a lista de comandos a partir dos tokens.
*/
int	build_commands(char **tokens, t_cmd **cmds, char **envp, int last_status)
{
	int		i;
	int		argc;
	char	*expanded_token;
	char	*final_token;

	if (!tokens || !tokens[0])
		return (*cmds = NULL, 0);
	*cmds = ft_calloc(1, sizeof(t_cmd));
	if (!(*cmds))
		return (-1);
	argc = 0;
	while (tokens[argc] && ft_strcmp(tokens[argc], "|") != 0)
		argc++;
	(*cmds)->argv = ft_calloc(argc + 1, sizeof(char *));
	if (!(*cmds)->argv)
		return (free(*cmds), -1);
	i = -1;
	while (++i < argc)
	{
		expanded_token = expand_vars(tokens[i], envp, last_status);
		if (is_quoted(expanded_token))
		{
			final_token = ft_substr(expanded_token, 1, ft_strlen(expanded_token) - 2);
			free(expanded_token);
		}
		else
			final_token = expanded_token;
		(*cmds)->argv[i] = final_token;
	}
	(*cmds)->in_fd = -1;
	(*cmds)->out_fd = -1;
	return ((*cmds)->next = NULL, 0);
}

/*
** @brief Libera a memória alocada para o array de tokens.
*/
void	free_tokens(char **tokens)
{
	if (tokens)
		ft_free_matrix(tokens);
}

/*
** @brief Verifica se uma string está contida entre aspas correspondentes.
*/
int	is_quoted(const char *s)
{
	int	len;

	if (!s)
		return (0);
	len = ft_strlen(s);
	if (len < 2)
		return (0);
	if ((s[0] == '\'' && s[len - 1] == '\'') || \
		(s[0] == '\"' && s[len - 1] == '\"'))
		return (1);
	return (0);
}

/*
** @brief Expande variáveis de ambiente e o status de saída em um token.
*/
char	*expand_vars(const char *token, char **envp, int last_status)
{
	char	*expanded;
	char	*var_value;

	if (token[0] != '$')
		return (ft_strdup(token));
	if (ft_strcmp(token, "$?") == 0)
		return (ft_itoa(last_status));
	var_value = get_env_value(envp, token + 1);
	if (var_value)
	{
		expanded = ft_strdup(var_value);
		free(var_value);
		return (expanded);
	}
	return (ft_calloc(1, sizeof(char)));
}