/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vinpache <vinpache@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 14:30:38 by vinpache          #+#    #+#             */
/*   Updated: 2025/10/29 14:30:39 by vinpache         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *get_env_val(t_env *env, const char *name)
{
    t_env *n = find_env(env, name);
    return (n ? n->value : NULL);
}

static char *join_paths(const char *base, const char *rest)
{
    char *tmp = ft_strjoin(base, "/");
    char *out;
    if (!tmp) return NULL;
    out = ft_strjoin(tmp, rest);
    free(tmp);
    return out;
}

static char *expand_tilde(t_env *env, const char *arg)
/* retorna mallocado: substitui "~" ou "~/..." por HOME; se HOME ausente, retorna NULL */
{
    const char *home = get_env_val(env, "HOME");
    if (!home) return NULL;
    if (arg[1] == '\0')           // "~"
        return ft_strdup(home);
    if (arg[1] == '/')            // "~/..."
        return join_paths(home, arg + 2);
    // "~algo" (expansão de usuários) não implementamos — comportamento de bash é opcional
    return ft_strdup(arg);
}

int	builtin_cd(char **args, t_env **env)
{
    char    *path = NULL;
    char    oldpwd[4096];
    char    newpwd[4096];

    if (getcwd(oldpwd, sizeof(oldpwd)) == NULL)
        oldpwd[0] = '\0';

    /* sem args → HOME */
    if (!args[1] || !args[1][0])
    {
        char *home = get_env_val(*env, "HOME");
        if (!home)
        {
            ft_putendl_fd("minishell: cd: HOME not set", 2);
            return 1;
        }
        path = ft_strdup(home);
    }
    else if (ft_strcmp(args[1], "-") == 0)
    {
        char *old = get_env_val(*env, "OLDPWD");
        if (!old)
        {
            ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
            return 1;
        }
        path = ft_strdup(old);
    }
    else if (args[1][0] == '~')
    {
        path = expand_tilde(*env, args[1]);
        if (!path)
        {
            ft_putendl_fd("minishell: cd: HOME not set", 2);
            return 1;
        }
    }
    else
        path = ft_strdup(args[1]);

    if (!path)
        return 1;

    if (chdir(path) != 0)
    {
        ft_putstr_fd("minishell: cd: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd(": ", 2);
        perror(NULL);            /* imprime msg do errno */
        free(path);
        return 1;
    }

    /* se foi "cd -" imprime o novo diretório (bash-like) */
    if (ft_strcmp(args[1] ? args[1] : "", "-") == 0)
    {
        if (getcwd(newpwd, sizeof(newpwd)) != NULL)
            ft_putendl_fd(newpwd, 1);
    }

    if (getcwd(newpwd, sizeof(newpwd)) != NULL)
    {
        if (oldpwd[0] != '\0')
            set_env_kv(env, "OLDPWD", oldpwd);
        set_env_kv(env, "PWD", newpwd);
    }

    free(path);
    return 0;
}
