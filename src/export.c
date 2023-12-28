#include "../include/minishell.h"
extern int g_return;
//idk if should go here

t_envp	*ft_lstlast_ev(t_envp *lst)
{
	if (!lst)
		return (0);
	while (lst)
	{
		if (lst->next == NULL)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_back_ev(t_envp **lst, t_envp *new) //int type
{
	t_envp	*t;

	t = *lst;
	if (!*lst)
		*lst = new;
	else
	{
		t = ft_lstlast_ev(*lst);
		t->next = new;
	}
}

char	*ft_getenv(char *env_name, t_envp *ep)
{
	while (ep)
	{
		if (!(ft_strcmp(ep->content[0], env_name)))
			return (ep->content[1]);
		ep = ep->next;
	}
	return (NULL);
}

void	print_export(t_envp *ep)
{
	if (!ep)
		return ;
	while (ep)
	{
		printf("declare -x ");
		if (ep->content[0])
			printf("%s", ep->content[0]);
		if (ep->content[1])
			printf("=\"%s\"\n", ep->content[1]);
		else
			printf("\n");
		ep = ep->next;
	}
}

int	do_exports(t_command *cmd, t_envp **ep)
{
	int	i;
	char	**content;
	int	ret;
	int	t;

	i = 1;
	content = NULL;
	ret = 0;
	t = 0;
	if (cmd->cmd_arg[1] == NULL)
	{
		print_export(*ep);
		return (0);
	}
	while (cmd->cmd_arg[i] != NULL)
	{
		content = split_env(cmd->cmd_arg[i]);
		if (content)
		{
			t = do_export(content[0], content[1], ep, cmd->cmd_arg[i]);
			if (t == 1)
				ret = 1;
			else if (t == -1)
				ret = -1;
			free_content(content);
			content = NULL;
		}
		else
			ret = g_return;
		i++;
	}
	return (ret);
}

 // give the test=test
int	do_export(char *name, char *value, t_envp **ep, char *cmd)
{
	t_envp *t;
	char	**content;

	t = *ep;
	if (!(ft_strcmp(name, "$?")))
	{
		if (value)
			printf("Minishell: export: `0=%s': not a valid identifier\n", value);
		else
			printf("Minishell: export: `0': not a valid identifier\n");
		g_return = 1;
		return (1);
	}
	else if (ft_isdigit(name[0]) || inv_env_name(name))
	{
		// if (value)
		// 	printf("Minishell: export: `%s=%s': not a valid identifier\n", name, value);
		// else
		// 	printf("Minishell: export: `%s': not a valid identifier\n", name);
		printf("Minishell: export: `%s': not a valid identifier\n", cmd);
		g_return = 1;
		return (1);
	}
	if (t)
	{
		while (t)
		{
			if (!(ft_strcmp(t->content[0], name)))
			{
				if ((t->content[1] && !(ft_strcmp(t->content[1], value))) || !value)
					return (0);
				ft_free(t->content[1]);
				t->content[1] = ft_strdup(value);
				return (0);
			}
			t = t->next;
		}
	}
	content = malloc(sizeof(char *) * 3);
	if (!(content))
	{
		perror("malloc failed in do_export");
		return (ft_fail_alloc(), -1);
	}
	content[0] = ft_strdup(name);
	content[1] = ft_strdup(value);
	content[2] = NULL;
	return (add_new_envp(ep, content));
}

/* //modified so do_export work with the simple_cmd format
void	exec_export(t_command *cmd, t_envp **env)
{

} */
