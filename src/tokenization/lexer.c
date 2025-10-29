/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:12:59 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/29 16:21:15 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token(t_token **lst, t_token_type type, char *value)
{
	t_token	*new;

	new = new_token(type, value);
	add_token_back(lst, new);
}

int	read_word(char *s, int i, t_token **lst)
{
	int		start;
	char	*word;

	start = i;
	while (s[i] && !ft_isspace(s[i]) && !ft_strchr("|<>", s[i]) && s[i] != '"'
		&& s[i] != '\'')
		i++;
	word = ft_substr(s, start, i - start);
	if (*word)
		add_token(lst, T_WORD, word);
	else
		free(word);
	return (i);
}

int	read_symbol(char *s, int i, t_token **lst)
{
	if (s[i] == '|')
		add_token(lst, T_PIPE, ft_strdup("|"));
	else if (s[i] == '<' && s[i + 1] == '<')
		(add_token(lst, T_HEREDOC, ft_strdup("<<")), i++);
	else if (s[i] == '>' && s[i + 1] == '>')
		(add_token(lst, T_REDIR_APPEND, ft_strdup(">>")), i++);
	else if (s[i] == '<')
		add_token(lst, T_REDIR_IN, ft_strdup("<"));
	else if (s[i] == '>')
		add_token(lst, T_REDIR_OUT, ft_strdup(">"));
	return (i + 1);
}

int	read_quoted(char *s, int i, t_token **lst)
{
	char	quote;
	int		start;
	char	*word;

	quote = s[i];   // 1. Guarda a aspa (ex: ') [MUDANÇA]
	start = i;      // 2. Marca o início NA aspa [MUDANÇA]
	i++;            // 3. Avança para o conteúdo [MUDANÇA]
	
	while (s[i] && s[i] != quote)
		i++;
	
	/* 4. Avança para DEPOIS da aspa final, se ela existir */
	if (s[i] == quote)
		i++;
	
	/* 5. Cria a substring incluindo as aspas (ex: '"ola"') */
	word = ft_substr(s, start, i - start);
	add_token(lst, T_WORD, word);
	
	/* 6. Retorna a nova posição 'i' (já está depois da aspa final) */
	return (i);
}

t_token	*tokenize(char *line)
{
	t_token *tokens;
	int i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		if (ft_isspace(line[i]))
			i++;
		else if (ft_strchr("|<>", line[i]))
			i = read_symbol(line, i, &tokens);
		else if (line[i] == '"' || line[i] == '\'')
			i = read_quoted(line, i, &tokens);
		else
			i = read_word(line, i, &tokens);
	}
	return (tokens);
}