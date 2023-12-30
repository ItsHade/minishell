
#include "../include/minishell.h"
extern int g_return;

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

int	isset(char const *set, char c)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*get_env_value(char *s, int dollar, t_envp *ep)
{
	int	i;
	char	*env_name;
	char	*env_value;
	
	env_name = NULL;
	env_value = NULL;
	i = dollar;
	i++;
	while (s[i] && is_env(s[i]))
			i++;
	env_name = ft_substr(s, dollar + 1, i - dollar - 1);
	if (!env_name)
		return (ft_fail_alloc(), NULL);
	env_value = ft_getenv(env_name, ep);
	ft_free(env_name);
	env_name = NULL;
	return (env_value);
}

// line : $rien    
int	add_dollar_str(char **line, int i, t_envp *ep)
{
	int		i_dollar;
	char	*env_value;
	char	*t1;
	char	*t2;

	//printf("avant! %s\n", *line);
	i_dollar = i;
	env_value = get_env_value(*line, i_dollar, ep);
	if (!env_value && g_return == -1)
		return (-1);
	i++;
	while ((*line)[i] && is_env((*line)[i]))
		i++;
	if (env_value || (!env_value
			&& (!(*line)[i] || (isset(" 	|$", (*line)[i])))))
	{		
		t1 = ft_substr(*line, 0, i_dollar);
		t2 = ft_substr(*line, i, ft_strlen(*line) - i);
		ft_free(*line);
		*line = NULL;
		i = i_dollar + ft_strlen(env_value);
		*line = ft_strjoinf(t1, env_value);
		*line = ft_strjoinf(*line, t2);
		ft_free(t2);
	}
	return (i);
}

int	is_printable(char c)
{
	if (c >= 33 && c <= 126)
		return (1);
	return (0);
}

int is_redir(char *s, int i)
{
	i--;
	if (is_printable(s[i]) && !isset("<>", s[i]))
	{
		while (i > 0 && s[i] && is_printable(s[i]) && !isset("<>", s[i]))
			i--;
	}
	if (s[i] && isset(" 	", s[i]))
	{
		while (i > 0 && s[i] && isset(" 	", s[i]))
			i--;
	}
	if (s[i] && (s[i] == '>' || s[i] == '<'))
		return (i);
	return (0);
}

int	check_amb_str(char *s)
{
	int	i;
	int sp;
	int	nsp;

	i = 0;
	sp = 0;
	nsp = 0;
	if (!s[0])
		return (1);
	while (s[i] && isset(" 	", s[i]))
			i++;
	if (i && !s[i])
		return (1);
	while (s[i])
	{
		if (s[i] && isset(" 	", s[i]))
		{
			while (s[i] && isset(" 	", s[i]))
				i++;
			sp++;
		}
		else if (s[i] && !isset(" 	", s[i]))
		{
			while (s[i] && !isset(" 	", s[i]))
				i++;
			nsp++;
		}
		if (sp == 1 && nsp == 2)
			return (1);	
	}
	return (0);
}

//no $? $$ 
int	is_ambiguous(char *s, int dl, t_envp *ep)
{
	int	is_r;
	char *t;
	int i;
	int	ret;

	t = NULL;
	i = dl;
	is_r = is_redir(s, dl);
	if (!is_r && !isset("<>", s[0]))
		return (0);
	while (s[i] && !isset(" 	", s[i]))
		i++;
	t = ft_substr(s, is_r + 1, i - is_r - 1);
	dl = 0;
	while (t[dl] && t[dl] != '$')
		dl++;
	if (add_dollar_str(&t, dl, ep) == -1)
		return (-1);
	ret = check_amb_str(t);
	ft_free(t);
	return (ret);
}

int	expand_line(char **line, t_envp *ep)
{
	int	i;
	int	is_amb;

	i = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == '\"')
            i = ft_strchr(*line + i + 1, '\"') - *line + 1;
        else if ((*line)[i] == '\'')
            i = ft_strchr(*line + i + 1, '\'') - *line + 1;
		else if ((*line)[i] == '$' && (*line)[i + 1])
		{
			if ((*line)[i + 1] && ((*line)[i + 1] == '$' || (*line)[i + 1] == '?'))
				i += 2;
			else if ((*line)[i + 1] && is_env((*line)[i + 1]))
			{
				is_amb = is_ambiguous((*line), i, ep);
				if (is_amb == -1)
					return (-1);
				else if (is_amb)
				{
					while ((*line)[i] && !isset(" 	", (*line)[i]))
						i++;
					printf("is_amb\n");
				}
				else
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
	ft_get_token_list(token_list, *line);
	ft_tokenize(token_list);
	return (0);
}
