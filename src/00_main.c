/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maburnet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 17:01:55 by maburnet          #+#    #+#             */
/*   Updated: 2023/12/22 17:01:57 by maburnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int g_return = 0;

//
void    ft_putinttab(int *tab, int nb)
{
	int i;

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
void    ft_puttab(char **tab)
{
	int i;

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
	t_token *current;

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

//in case of failled malloc (nb should be -1)
void	ft_exit(t_data *data, char *line, int nb)
{
	clear_ep(&data->env);
	rl_clear_history();
	ft_free(line);
	line = NULL;
	exit(nb);
}

char *ft_get_line(t_data *data, char *line)
{
	line = readline("\033[4;32mMinishell\033[0;32m>\033[0;37m ");
	if (line != NULL)
	{
		if (line && *line)
			add_history(line);
		if (line[0] == '\0')
		{
			free(line);
			return (NULL);
		}
		if (quote_error(line))
			return (NULL);
		return (line);
	}
	else
	{
		ft_putstr_fd("exit\n", 2);
		clear_ep(&data->env);
		rl_clear_history();
		exit(g_return);
	}
}

int	minishell(t_data *data)
{
	char	*line;
	t_token	*token_list;

	token_list = NULL;
	line = NULL;
	while (1)
	{
		if (g_return == -1)
			ft_exit(data, NULL, -1);
		signal(SIGINT, ft_signal);
		signal(SIGQUIT, SIG_IGN);
		line = ft_get_line(data, line);
		if (!line)
			continue;
		if (ft_lexer(&token_list, &line, data) == -1)
			ft_exit(data, line, -1);
		if (!line)
			continue;
		ft_free(line);
		if (syntax_error(&token_list))
		{
			ft_lstclear(&token_list);
			continue ;
		}
		if (ft_parsing(&token_list, data) == -1) 
			continue;
		ft_lstclear(&token_list);
		if (ft_execute(data, &data->env) == -1)
		{
			ft_freecmdtable(data);
			ft_putstr_fd("test\n", 2); //
			ft_exit(data, NULL, -1);
		}
		ft_freecmdtable(data);
	}
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_data	data;

	data.env = NULL;
	set_env(&data.env, envp);
	minishell(&data);
	return (0);
}
