
#include "../include/minishell.h"

int	ft_nb_of_cmd(t_token **token_list)
{
	int		count;
	t_token	*current;

	count = 0;
	current = *token_list;
	if (current != NULL)
		count = 1;
	while (current)
	{
		if (current->label == PIPE)
			count++;
		current = current->next;
	}
	return (count);
}

void	ft_go_to_next_cmd(t_token **next_cmd)
{
	while (*next_cmd && (*next_cmd)->label != PIPE)
		*next_cmd = (*next_cmd)->next;
	if (*next_cmd && (*next_cmd)->next && (*next_cmd)->label == PIPE)
		*next_cmd = (*next_cmd)->next;
}

int	ft_fill_cmd_table(t_token **token_list, t_data *data)
{
	int	i;
	t_token *next_cmd;

	i = 0;
	next_cmd = *token_list;
	data->nb_cmd = ft_nb_of_cmd(token_list);
	data->commands = ft_calloc(sizeof(t_command), (data->nb_cmd + 1));
	if (!data->commands)
		return (perror("ft_calloc"), ft_fail_alloc(), -1);
	while (i < data->nb_cmd)
	{
		ft_get_cmd_info(&next_cmd, &data->commands[i].nb_arg, &data->commands[i].nb_io);
		if (ft_get_redir(&data->commands[i], &next_cmd) == -1)
			return (-1);
		if (ft_get_args(&data->commands[i], &next_cmd) == -1)
			return (-1);
		if (next_cmd != NULL)
			ft_go_to_next_cmd(&next_cmd);
		i++;
	}
	return (0);
}

void	ft_check_tokens(t_token *current, int *cmd)
{
	if (current->label == HEREDOC && current->next)
		current->next->label = LIMITER;
	if ((current->label == I_RDR || current->label == O_RDR || current->label == OAPPEND) && current->next)
		current->next->label = IO_FILE;
	if (current->label == PIPE && current->next && current->next->label == WORD)
		current->next->label = CMD;
	if (current->label == WORD && current->index == 0)
	{
		current->label = CMD;
		*cmd = 1;
	}
	if (current->label == CMD && current->next && current->next->label == WORD)
		current->next->label = ARG;
	if (current->label == ARG && current->next && current->next->label == WORD)
		current->next->label = ARG;
	if (current->label == IO_FILE && current->next && current->next->label == WORD)
		current->next->label = ARG;
	if ((current->label == WORD || current->label == ARG) && cmd == 0)
	{
		current->label = CMD;
		*cmd = 1;
	}
	if (current->label == PIPE)
		*cmd = 0;
}

int	ft_parsing(t_token **token_list, t_data *data)
{
	t_token	*current;
	int		cmd;

	current = *token_list;
	cmd = 0;
	while (current)
	{
		if (current->index == 0 && current->label == PIPE)
			return (-1);
		ft_check_tokens(current, &cmd);
		current = current->next;
	}
	if (ft_fill_cmd_table(token_list, data) == -1)
		return (-1);
	if (data->nb_cmd < 1)
		return (ft_freecmdtable(data), ft_lstclear(token_list), -1);
	return (0);
}
