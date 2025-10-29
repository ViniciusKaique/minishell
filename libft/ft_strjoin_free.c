#include "libft.h"

char    *ft_strjoin_free(char *s1, char const *s2)
{
    char    *new_str;

    if (!s1)
        return (ft_strdup(s2));
    if (!s2)
        return (s1);
    new_str = ft_strjoin(s1, s2);
    free(s1);
    return (new_str);
}