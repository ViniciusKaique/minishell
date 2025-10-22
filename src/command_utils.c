/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 18:12:20 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/22 18:50:51 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command *new_command(void)
{
    t_command *cmd;
    
    cmd = malloc(sizeof(t_command));
    if(!cmd)
        return(NULL);
    cmd->args = NULL;
    cmd->redirects = NULL;
    cmd->next = NULL;
    return(cmd);
}

void	add_command_back(t_command **list, t_command *new)
{
	t_command	*tmp;

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