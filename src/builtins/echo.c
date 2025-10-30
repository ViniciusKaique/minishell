/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 14:55:15 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/30 14:55:24 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_newline_flag(char **args, int *i_ptr)
{
	int	j;
	int	newline;

	newline = 1;
	while (args[*i_ptr] && ft_strncmp(args[*i_ptr], "-n", 2) == 0)
	{
		j = 1;
		while (args[*i_ptr][j] == 'n')
			j++;
		if (args[*i_ptr][j] != '\0')
			break ;
		newline = 0;
		(*i_ptr)++;
	}
	return (newline);
}

static void	print_echo_args(char **args, int i)
{
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
}

int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = check_newline_flag(args, &i);
	print_echo_args(args, i);
	if (newline)
		ft_putchar_fd('\n', 1);
	return (0);
}
