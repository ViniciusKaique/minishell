#include "../include/minishell.h"

/*
** @brief Implementa o comando 'echo'.
*/
int	builtin_echo(char **argv)
{
	int	i = 1;
	int	n_flag = 0;

	if (argv[i] && ft_strcmp(argv[i], "-n") == 0) // Suporte à flag -n [cite: 114, 864]
	{
		n_flag = 1;
		i++;
	}
	while (argv[i])
	{
		write(1, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	return (0);
}

/*
** @brief Implementa o comando 'pwd'.
*/
int	builtin_pwd(void)
{
	char	cwd[1024]; // Buffer estático para simplicidade

	if (getcwd(cwd, sizeof(cwd)) != NULL) // Usa getcwd [cite: 126, 869]
	{
		printf("%s\n", cwd);
		return (0);
	}
	perror("pwd");
	return (1);
}

/*
** @brief Implementa o comando 'cd'.
*/
int	builtin_cd(char **argv, char ***envp)
{
	char *path, *old_pwd_val, *current_pwd;

	old_pwd_val = get_env_value(*envp, "PWD");
	
	if (!argv[1])
	{
		path = get_env_value(*envp, "HOME"); // Se sem arg, usa HOME [cite: 119, 868]
		if (!path) { write(2, "cd: HOME not set\n", 17); free(old_pwd_val); return (1); }
	}
	else
		path = ft_strdup(argv[1]);

	if (chdir(path) != 0) // Altera o diretório [cite: 120]
	{
		perror(path);
		free(path);
		free(old_pwd_val);
		return (1);
	}
	
	current_pwd = getcwd(NULL, 0);
	if (old_pwd_val)
	{
		set_env_value(envp, "OLDPWD", old_pwd_val); // Atualiza OLDPWD [cite: 121]
		free(old_pwd_val);
	}
	if (current_pwd)
	{
		set_env_value(envp, "PWD", current_pwd); // Atualiza PWD [cite: 121]
		free(current_pwd);
	}
	free(path);
	return (0);
}

/*
** @brief Implementa o comando 'env'.
*/
int	builtin_env(char **envp)
{
	int	i = 0;
	while (envp && envp[i])
	{
		printf("%s\n", envp[i]); // Imprime cada variável de ambiente [cite: 139, 872]
		i++;
	}
	return (0);
}

/*
** @brief Implementa o comando 'exit'.
*/
int builtin_exit(char **argv, char **envp, struct termios *termios)
{
    int exit_code;

    exit_code = 0;
    printf("exit\n");
    if (argv[1])
        exit_code = ft_atoi(argv[1]);
    
    // RESTAURA O TERMINAL ANTES DE SAIR
    tcsetattr(STDIN_FILENO, TCSANOW, termios);

    ft_free_matrix(envp);
    // ... outras limpezas ...
    exit(exit_code);
}