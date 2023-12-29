#include "../include/minishell.h"

void	ft_free(void *to_free)
{
	if (to_free)
		free(to_free);
}

void	ft_freetab(char **tab)
{
	int	a;

	a = 0;
	if (!tab)
		return ;
	while (tab[a] != NULL)
	{
		ft_free(tab[a]);
		a++;
	}
	ft_free(tab[a]);
	ft_free(tab);
}

void	ft_freemalloc(char **strs, int a)
{
	while (a >= 0)
	{
		free(strs[a]);
		a--;
	}
	free(strs);
}

void	ft_freecmdtable(t_data *data)
{
	int	i;

	i = 0;
	while(i < data->nb_cmd)
	{
		ft_freetab(data->commands[i].cmd_arg);
		ft_freetab(data->commands[i].files);
		ft_free(data->commands[i].redir);
		//ft_free(data->commands[i].is_null);
		i++;
	}
	free(data->commands);
}
