/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzhuang <yzhuang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 18:46:37 by yzhuang           #+#    #+#             */
/*   Updated: 2023/12/30 18:51:49 by yzhuang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern int	g_return;

int	ft_get_token(char *value)
{
	if (ft_strcmp(value, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(value, "\"") == 0)
		return (DQUOTE);
	else if (ft_strcmp(value, "'") == 0)
		return (SQUOTE);
	else if (ft_strcmp(value, "<") == 0)
		return (I_RDR);
	else if (ft_strcmp(value, ">") == 0)
		return (O_RDR);
	else if (ft_strcmp(value, "<<") == 0)
		return (HEREDOC);
	else if (ft_strcmp(value, ">>") == 0)
		return (OAPPEND);
	else if (ft_strncmp(value, "<<<", 3) == 0)
		return (SYNTAX);
	else if (ft_strncmp(value, ">>>", 3) == 0)
		return (SYNTAX);
	else
		return (WORD);
}

int	ft_tokenize(t_token **token_list)
{
	t_token	*current;
	int		i;

	current = *token_list;
	i = 0;
	while (current)
	{
		current->index = i;
		current->label = ft_get_token(current->value);
		current = current->next;
		i++;
	}
	return (0);
}

int	ft_lexer(t_token **token_list, char **line, t_data *data)
{
	if (expand_line(line, data->env) == -1)
		return (-1);
	if (!((*line)[0]))
	{
		ft_free(*line);
		*line = NULL;
		return (0);
	}
	ft_get_token_list(token_list, *line);
	ft_tokenize(token_list);
	return (0);
}
