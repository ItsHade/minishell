#include "../include/minishell.h"

//
void	ft_putinttab(int *tab, int nb)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (i < nb)
	{
		printf("%d\n", tab[i]);
		i++;
	}
}

//
void	ft_puttab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i] != NULL)
	{
		ft_putstr(tab[i]);
		ft_putstr("\n");
		i++;
	}
}

//
void	ft_putlst(t_token **token_list)
{
	t_token	*current;

	current = *token_list;
	while (current != NULL)
	{
		printf("Value: \033[1;31m%s\033[1;0m\n", (char *)current->value);
		printf("Label: %d\n", current->label);
		printf("Index: %d\n", current->index);
		current = current->next;
	}
}

//
void	ft_putcmdtable(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_cmd)
	{
		ft_puttab(data->commands[i].cmd_arg);
		i++;
	}
}