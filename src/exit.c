#include "../include/minishell.h"

extern int g_return;

int ft_str_is_num(char *s)
{
    int i;

    i = 0;
    if (!s)
        return (0);
    if (s[i] == '-' || s[i] == '+')
        i++;
    while (s[i])
    {
        if (ft_isdigit(s[i]) == 0)
            return (-1);
        i++;
    }
    return (0);
}

int do_exit(t_data *data, t_envp **ep, int i, int is_pipe)
{
    long res;
    int too_big;

    res = 0;
    too_big = 0;
    if (is_pipe == 0)
        ft_putstr_fd("exit\n", 2);
    if (data->commands[i].cmd_arg[1] == NULL)
        res = g_return;
    else
    {
        res = ft_atol(data->commands[i].cmd_arg[1], &too_big);
        if (ft_strlen(data->commands[i].cmd_arg[1]) > 20
            || ft_str_is_num(data->commands[i].cmd_arg[1]) == -1
            || too_big == 1)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(data->commands[i].cmd_arg[0], 2);
            ft_putstr_fd(": ", 2);
            ft_putstr_fd(data->commands[i].cmd_arg[1], 2);
            ft_putstr_fd(": numeric argument required\n", 2);
            res = 2;
        }
        else if (data->commands[i].cmd_arg[1] != NULL && data->commands[i].cmd_arg[2] != NULL)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(data->commands[i].cmd_arg[0], 2);
            ft_putstr_fd(": too many arguments\n", 2);
            return (1);
        }
        else if (data->commands[i].cmd_arg[1] != NULL)
        {
            res = (res % 256 + 256) % 256;
        }
    }
    ft_close_std(data);
    ft_freecmdtable(data);
    clear_ep(ep);
    rl_clear_history();
    fprintf(stderr, "return: %ld\n", res); // remove this after all testing
    exit(res);
}