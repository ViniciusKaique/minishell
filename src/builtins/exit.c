#include "minishell.h"

static int	is_numeric(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	if (*s == '+' || *s == '-')
		s++;
	if (!*s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **args)
{
	int		count;
	long	code;

	count = 0;
	while (args[count])
		count++;
	ft_putendl_fd("exit", 1);
	if (count == 1)
		exit(0);
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit(2);
	}
	if (count > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (1);
	}
	code = ft_atoi(args[1]) % 256;
	if (code < 0)
		code += 256;
	exit((int)code);
}
