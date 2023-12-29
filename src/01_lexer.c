
#include "../include/minishell.h"

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
	else if (ft_strcmp(value, "><") == 0)
		return (SYNTAX);
	else if (ft_strcmp(value, "<>") == 0)
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

int	add_dollar_str(char **line, int i, t_envp *ep)
{
	int		i_dollar;
	char	*env_name;
	char	*t1;
	char	*t2;

	i_dollar = i;
	i++;
	while ((*line)[i] && is_env((*line)[i]))
		i++;
	env_name = ft_substr(*line, i_dollar + 1, i - i_dollar - 1);
	if (!env_name)
		return (-1);
	if (ft_getenv(env_name, ep) || (!ft_getenv(env_name, ep)
		&& (!(*line)[i] || (*line)[i] == ' ' || (*line)[i] == '	' || (*line)[i] == '|' || (*line)[i] == '$')))
	{		
		t1 = ft_substr(*line, 0, i_dollar);
		t2 = ft_substr(*line, i, ft_strlen(*line) - i);
		ft_free(*line);
		*line = NULL;
		i = i_dollar + ft_strlen(ft_getenv(env_name, ep));
		*line = ft_strjoinf(t1, ft_getenv(env_name, ep));
		*line = ft_strjoinf(*line, t2);
		ft_free(t2);
	}
	ft_free(env_name);
	env_name = NULL;
	return (i);
}

int	expand_line(char **line, t_envp *ep)
{
	int	i;

	i = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == '\"')
			i = ft_strchr(*line + i, '\"') - *line + 1;
		else if ((*line)[i] == '\'')
			i = ft_strchr(*line + i, '\'') - *line + 1;
		else if ((*line)[i] == '$' && (*line)[i + 1])
		{
			if ((*line)[i + 1] && ((*line)[i + 1] == '$' || (*line)[i + 1] == '?'))
				i += 2;
			else if ((*line)[i + 1] && is_env((*line)[i + 1]))
			{
				i = add_dollar_str(line, i, ep);
				if (i == -1)
					return (-1);
			}
			else
				i++;
		}
		else
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
	ft_get_token_list(token_list, *line, " 	", "|<>");
	ft_tokenize(token_list);
	return (0);
}
