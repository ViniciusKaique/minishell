/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 16:36:21 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/23 15:51:59 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{

	//t_command	*cmd1;
	//t_command	*cmd2;
	// char	*input;
	(void)argc;
	(void)argv;
	(void)envp;
	// while (1)
	// {
	// 	input = readline("minishell$ ");
	// 	if (!input)
	// 	{
	// 		printf("exit\n");
	// 		break ;
	// 	}
	// 	if (*input)
	// 		add_history(input);
	// 	printf("Você digitou: %s\n", input);
	// 	free(input);
	// }
	// return (0);
	// t_command *cmd =  new_command();
	// if(!cmd)
	// 	return(1);
	// cmd->args = (char *[]){"ls", "-la", NULL};
	// printf("cmd: %s %s\n", cmd->args[0], cmd->args[1]);
	// free(cmd);
	// t_redirect *r = new_redirect(R_OUT_TRUNC, "saida.txt");
	// printf("tipo: %d | arquivo: %s\n", r->type, r->file);
	// free(r);
	// char **paths = ft_split(getenv("PATH"), ':');
	// int i = 0;
	// if (!paths)
	//     return (1);
	// while (paths[i])
	// {
	//     printf("%s\n", paths[i]);
	//     i++;
	// }
	// i = 0;
	// while (paths[i])
	//     free(paths[i++]);
	// free(paths);
	// char *path = find_command_path("ls");
	// if (path)
	// {
	// 	printf("Encontrado: %s\n", path);
	// 	free(path);
	// }
	// else
	// 	printf("Não encontrado\n");
	// char *args[] = {"ls", "-la", NULL};
	// exec(args, envp);

	t_token	*tokens;
	t_token	*tmp;

	tokens = tokenize("cat" "< 'in.txt' | grep .c >> out.txt");
	tmp = tokens;
	while (tmp)
	{
		printf("type: %d\tvalue: %s\n", tmp->type, tmp->value);
		tmp = tmp->next;
	}
	free_tokens(tokens);
	return (0);
}
