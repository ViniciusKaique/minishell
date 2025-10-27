/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 18:12:20 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/22 18:50:52 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirect	*new_redirect(t_redirect_type type, char *file)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));      // 1️⃣ reserva memória
	if (!redir)                              // 2️⃣ verifica se deu erro
		return (NULL);
	redir->type = type;                      // 3️⃣ salva o tipo (>, >>, <, <<)
	redir->file = file;                      // 4️⃣ armazena o nome do arquivo
	redir->next = NULL;                      // 5️⃣ nenhum próximo redir ainda
	return (redir);                          // 6️⃣ retorna o novo nó
}

// adiciona um novo redirecionamento no final da lista
void	add_redirect_back(t_redirect **list, t_redirect *new)
{
	t_redirect	*tmp;

	if (!list || !new)
		return ;
	if (!*list)
	{
		*list = new;     // se a lista está vazia, new vira o primeiro
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next; // caminha até o último
	tmp->next = new;     // conecta o novo no final
}