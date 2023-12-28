#include "../include/minishell.h"

extern int g_return;

int	print_envp(t_envp *envp)
{
	if (!envp)
		return (-1);
	while (envp)
	{
		if (envp->content[0] && envp->content[1])
			printf("%s=%s\n", envp->content[0], envp->content[1]);
		envp = envp->next;
	}
	return (0);
}

void	free_content(char **content)
{
	if (!content)
		return ;
	ft_free(content[0]);
	ft_free(content[1]);
	content[0] = NULL;
	content[1] = NULL;
	free(content);

}		

char	**split_env(char *s) //if !s?  =ok  =  ok  ok=  
{
	int		i;
	char	**content;
	int	tlen;

	i = 0;
	tlen = 0;
	content = NULL;
	if (s[0] == '=' || !s[0])
	{
		printf("Minishell: export: `%s': not a valid identifier\n", s);
		g_return = 1;
		return (NULL);
	}
	content = ft_calloc(sizeof(char *), 3);
	if (!content)
	{
		perror("malloc");
		return (NULL);
	}
	while (s[i] && s[i] != '=')
		i++;
	if (s[i] == '=')
	{
		content[0] = ft_substr(s, 0, i);
		tlen = ft_strlen(s) - i - 1;
		if (tlen)
			content[1] = ft_substr(s, i + 1, tlen);
		else
			content[1] = NULL;
		content[2] = NULL;
	}
	else
	{
		content[0] = ft_substr(s, 0, i);
		content[1] = NULL;
		content[2] = NULL;
	}
	return (content);
}

int	add_new_envp(t_envp **ep, char **content)
{
	t_envp *new_ep;

	new_ep = ft_calloc(sizeof(t_envp), 1);
	if (!new_ep)
	{
		perror("Malloc failed in add_new_ep");
		clear_ep(ep);
		g_return = -1;
		return (-1);
	}
	new_ep->content = content; //split_env(s);
	//printf("content0=%s, content1=%s\n", new_ep->content[0], new_ep->content[1]);
    new_ep->next = NULL;
	ft_lstadd_back_ev(ep, new_ep);
	return (0);
}

int	ft_get_shell_level(t_envp **ep)
{
	int	shlvl;
	char *s_lvl;
	char *tmp;

	tmp = ft_getenv("SHELL", *ep); 
	if (!tmp)
		return (-1);
	if (ft_strcmp(tmp, "minishell") != 0) 
		return (-1);
	tmp = ft_getenv("SHLVL", *ep);
	if (!tmp)
		return (-1);
	shlvl = ft_atoi(tmp);
	shlvl++;
	s_lvl = ft_itoa(shlvl);
	if (!s_lvl)
		return (-1);
	do_export("SHLVL", s_lvl, ep, NULL); //NULL AV
	free(s_lvl);
	return (0);
}

int	set_env(t_envp **ep, char **envp)
{
	int	i;
	char *s;

	i = 0;
	s = NULL;
	if (envp == NULL || envp[0] == NULL) 
	{
		ft_putstr_fd("Launched with env -i ?\n", 2);
		s = ft_strjoin("PWD=", getcwd(s, 1024));
		if (!s)
			return (ft_fail_alloc(), -1);
		if (add_new_envp(ep, split_env(s)) == -1)
			return (-1);
		free(s);
		s = ft_strdup("SHLVL=1");
		if (!s)
			return (ft_fail_alloc(), -1);
		if (add_new_envp(ep, split_env(s)) == -1)
			return (-1);
		free(s);
	}
	while (envp[i])
	{
		if (add_new_envp(ep, split_env(envp[i])) == -1)
			return (-1);
		i++;
	}
	if (ft_get_shell_level(ep) == 0)
		do_export("SHELL", "minishell", ep, NULL);
	return (0);
}

//modified removed printf
void clear_ep(t_envp **ep)
{
    t_envp *t;
    int    i;

    t = NULL;
    while (*ep)
    {
        if ((*ep)->next)
            t = (*ep)->next;
        i = 0;
        while(i < 2)
        {
            ft_free((*ep)->content[i]);
            (*ep)->content[i] = NULL;
            i++;
        }
        ft_free((*ep)->content);
        ft_free(*ep);
        *ep = t;
        t = NULL;
    }
    ep = NULL;
}