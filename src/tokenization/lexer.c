/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:28:11 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/30 18:28:58 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_symbol(char *s, int i, t_token **lst)
{
	if (s[i] == '|')
		add_token(lst, T_PIPE, ft_strdup("|"), 0);
	else if (s[i] == '<' && s[i + 1] == '<')
		(add_token(lst, T_HEREDOC, ft_strdup("<<"), 0), i++);
	else if (s[i] == '>' && s[i + 1] == '>')
		(add_token(lst, T_REDIR_APPEND, ft_strdup(">>"), 0), i++);
	else if (s[i] == '<')
		add_token(lst, T_REDIR_IN, ft_strdup("<"), 0);
	else if (s[i] == '>')
		add_token(lst, T_REDIR_OUT, ft_strdup(">"), 0);
	return (i + 1);
}

static int	handle_quoted_part(char *s, int i, char **full_word,
		int *has_quotes)
{
	char	*word_part;
	int		start;
	char	quote;

	*has_quotes = 1;
	quote = s[i];
	start = i;
	i++;
	while (s[i] && s[i] != quote)
		i++;
	if (s[i] == quote)
		i++;
	word_part = ft_substr(s, start, i - start);
	*full_word = ft_strjoin_free(*full_word, word_part);
	free(word_part);
	return (i);
}

static int	handle_unquoted_part(char *s, int i, char **full_word)
{
	char	*word_part;
	int		start;

	start = i;
	while (s[i] && !ft_isspace(s[i]) && !ft_strchr("|<>", s[i]) && s[i] != '"'
		&& s[i] != '\'')
		i++;
	word_part = ft_substr(s, start, i - start);
	*full_word = ft_strjoin_free(*full_word, word_part);
	free(word_part);
	return (i);
}

static int	read_complex_word(char *s, int i, t_token **lst)
{
	char	*full_word;
	int		has_quotes;

	full_word = ft_strdup("");
	has_quotes = 0;
	while (s[i] && !ft_isspace(s[i]) && !ft_strchr("|<>", s[i]))
	{
		if (s[i] == '"' || s[i] == '\'')
			i = handle_quoted_part(s, i, &full_word, &has_quotes);
		else
			i = handle_unquoted_part(s, i, &full_word);
	}
	add_token(lst, T_WORD, full_word, has_quotes);
	return (i);
}

t_token	*tokenize(char *line)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		if (ft_isspace(line[i]))
			i++;
		else if (ft_strchr("|<>", line[i]))
			i = read_symbol(line, i, &tokens);
		else
			i = read_complex_word(line, i, &tokens);
	}
	return (tokens);
}
