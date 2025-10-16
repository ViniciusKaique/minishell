/*
** Compara as strings s1 e s2.
** Retorna 0 se forem iguais, ou a diferença entre os primeiros
** caracteres diferentes.
*/
int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return (*(unsigned char *)s1 - *(unsigned char *)s2);
}