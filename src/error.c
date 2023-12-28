#include "../include/minishell.h"

extern int g_return;

void    ft_error(char *s, char *err)
{
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(s, 2);
    perror(err);
}

void    ft_fail_alloc(void)
{
    g_return = -1;
}
