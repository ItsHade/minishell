#include "../include/minishell.h"

extern int g_return;

t_exp	*ft_lstlast_exp(t_exp *lst)
{
	if (lst == NULL || lst->next == NULL)
		return (lst);
	while (lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_back_exp(t_exp **lst, t_exp *new)
{
	t_exp	*cur;

	cur = *lst;
	if (lst != NULL && *lst != NULL)
	{
		cur = ft_lstlast_exp(*(lst));
		cur->next = new;
		return ;
	}
	*lst = new;
}

void	clear_exp(t_exp **exp)
{
	t_exp	*t;

	t = NULL;
	if (exp)
	{
		while (*exp)
		{
			if ((*exp)->next)
				t = (*exp)->next;
			ft_free((*exp)->s);
			(*exp)->s = NULL;
			ft_free(*exp);
			*exp = NULL;
			*exp = t;
			t = NULL;
		}
	}
	*exp = NULL;
}

int	add_new_exp(t_exp **exp, char *str) //, int start, int n)
{
	t_exp	*new_exp;

	new_exp = malloc(sizeof(t_exp));
	if (!new_exp)
	{
		perror("Malloc failed in add_new_exp");
		clear_exp(exp);
		g_return = -1;
		return (-1);
	}
	//new_exp->s = ft_substr(s, start, n);
	new_exp->s = str;
	new_exp->next = NULL;
	ft_lstadd_back_exp(exp, new_exp);
	return (0);
}

int	add_exp_value(t_exp **exp, char *env_name, t_envp *ep)
{
	char	*env_value;
	char	*t;

	t = NULL;
	env_value = NULL;
	t = ft_getenv(env_name, ep);
	if (t)
		env_value = ft_strdup(t);
	if (env_value || env_name[0] == '?')
	{
		if (env_name[0] == '?')
		{
			t = ft_itoa(g_return);
			if (!t)
				return (-1);
		}
		else
			t = env_value;
		if (add_new_exp(exp, t) == -1)
			return (-1);
	}
	ft_free(env_name);
	return (0);
}

int	do_dollar(t_exp **exp, char *s, int i, t_envp *ep)
{
	int		i_dollar;
	char	*env_name;

	i_dollar = i;
	i++;
	if (s[i] && (s[i] == '$' || s[i] == '?'))
	{
		env_name = ft_substr(s, i, 1); //given $ or ?
		if (add_exp_value(exp, env_name, ep) == -1)
			return (-1);
		i++;
	}
	else if (s[i] && ft_isdigit(s[i]))
			i++;
	else if (s[i] && is_env(s[i]))
	{
		while (s[i] && is_env(s[i]))
			i++;
		env_name = ft_substr(s, i_dollar + 1, i - i_dollar - 1);
		if (add_exp_value(exp, env_name, ep) == -1)
			return (-1);
	}
	env_name = NULL;
	return (i);
}

int	do_dq(t_exp **exp, char *s, int i, t_envp *ep)
{
	int	start;

	i++;
	if (s[i] == '\"')
		return (i + 1);
	while (s[i] && s[i] != '\"')
	{
		if (s[i] == '$' && s[i + 1] && (is_env(s[i + 1]) || s[i + 1] == '$' || s[i + 1] == '?'))
		{
			i = do_dollar(exp, s, i, ep); //le prochain, " ou } etc
			if (i == -1)
				return (-1);
		}	
		else
		{
			start = i;
			if (s[i] == '$') //a v
				i++;
			while (s[i] && s[i] != '\"' && s[i] != '$')
				i++;
			if (add_new_exp(exp, ft_substr(s, start, i - start)) == -1)
				return (-1);
		}
	}
	return (i + 1);//apres ""
}

int	do_not_exp(t_exp **exp, char *s, int i) //peut static
{
	int	start;

	if (s[i] == '\'')
	{
		i++;
		start = i;
		while (s[i] && s[i] != '\'')
			i++;
		if (s[i] && s[i] == '\'' && i != start)
		{
			if (add_new_exp(exp, ft_substr(s, start, i - start)) == -1)
				return (-1);
		}	
		i++;
	}
	else
	{
		start = i;
		if (s[i] == '$')
			i++;
		if (s[i] && s[i] == '$')
			i++;
		while (s[i] && s[i] != '\'' && s[i] != '\"' && s[i] != '$')
			i++;
		if (add_new_exp(exp, ft_substr(s, start, i - start)) == -1)
			return (-1);
	}
	return (i);
}

int	do_quote_exp(t_exp **exp, char *s, t_envp *ep)
{
	int	i;

	i = 0;
	if (s[0] == '~')
	{
		i = do_tilde(exp, s, ep);
		if (i == -1)
			return (-1); // MALLOC a MAJ
	}
	while (s[i])
	{
		if (s[i] == '\"')
		{
			i = do_dq(exp, s, i, ep);
			if (i == -1)
				return (-1);
		}	
		else if (s[i] == '$' && s[i + 1] && (is_env(s[i + 1])
				|| s[i + 1] == '?' || s[i + 1] == '\"' || s[i + 1] == '\''))
		{
			if (s[i + 1] == '\"' || s[i + 1] == '\'')
				i++;
			else
			{
				i = do_dollar(exp, s, i, ep);
				if (i == -1)
					return (-1);
			}
		}
		else
		{
			i = do_not_exp(exp, s, i);
			if (i == -1)
				return (-1);
		}
	}
	return (0);
}

int	do_tilde(t_exp **exp, char *s, t_envp *ep)
{
	char	*t;
	int	start;
	int	i;
	char	*substr;

	substr = NULL;
	t = NULL;
	i = 1;
	start = 0;
	if (s[0] == '~' && (!s[1] || s[1] == '/'))
		t = ft_getenv("HOME", ep);
	if (t)
		start = 1;
	while (s[i] && s[i] != '\'' && s[i] != '\"' && s[i] != '$')
		i++;
	substr = ft_substr(s, start, i - start);
	t = ft_strjoin(t, substr);
	ft_free(substr);
	if (add_new_exp(exp, t) == -1)
		return (-1);
	return (i);
}

int	quote_or_dollar(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"' || s[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

void	print_exp(t_exp *exp)
{
	if (!exp)
		return ;
	while (exp)
	{
		if (exp->s)
			ft_putstr_fd(exp->s, 1);
		exp = exp->next;
	}
}

char	*exp_to_str(t_exp *exp)
{
	char	*s;

	s = NULL;
	while (exp)
	{
		s = ft_strjoinf(s, exp->s);
		if (!s)
			return (NULL);
		exp = exp->next;
	}
	return (s);
}

int	is_option_n(char *s)
{
	int	i;

	i = 0;
	if (s[0] == '-' && s[1] == 'n' )
	{
		i += 2;
		while (s[i] && s[i] == 'n')
			i++;
		if (s[i])
			return (0);
	}
	else
		return (0);
	return (1);
}

int	do_echo(t_command *cmd)
{
	int	option_n;
	int	i;

	option_n = 0;
	i = 1;
	while (cmd->cmd_arg[i] != NULL)
	{
		if (!is_option_n(cmd->cmd_arg[i]))
			break ;
		else
			option_n = 1;
		i++;
	}
	while (cmd->cmd_arg[i] != NULL)
	{
		if (cmd->cmd_arg[i][0])
			ft_putstr(cmd->cmd_arg[i]);
		if (cmd->cmd_arg[i + 1] != NULL)
			ft_putstr(" ");
		i++;
	}
	if (!option_n)
		ft_putstr("\n");
	return (0);
}
