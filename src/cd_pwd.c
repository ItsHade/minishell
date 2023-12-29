
#include "../include/minishell.h"

char	*get_ab_path(char *path) // return malloquÃ©
{
	char	*t;
	char	*filename;
	char	*cwd;

	cwd = NULL;
	t = NULL;
	filename = NULL;
	cwd = getcwd(cwd, 0); //  /Users/Documents/2711
	if (path[0] == '.' && path[1] == '/')
		filename = ft_substr(path, 1, ft_strlen(path) - 1); // av /filename
	else
		filename = ft_strjoin("/", path);
	if (!filename)
		return (perror("malloc"), ft_fail_alloc(), NULL);
	t = ft_strjoinf(cwd, filename);
	free(filename);
	if (!t)
		return (NULL);
	cwd = NULL;
	return (t);
}

int	do_cd(char *path, t_envp **ep)
{
	DIR	*dirp;
	char	*t;
	char	*original_pwd;
	char	*cwd;
	int	f;

	cwd = NULL;
	f = 1;
	original_pwd = NULL;
	t = NULL;
	if (path == NULL || (path[0] == '~' && (!path[1] || path[1] == '/')) )
	{
		t = ft_getenv("HOME", *ep);
		f = 0;
		if (!t)
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	}
	else if ((path[0] == '.' && path[1] == '/') || (path[0] != '.' && path[0] != '/')) // ./   ou document  ou ~/documents
		t = get_ab_path(path);
	else
		t = ft_strdup(path);
	if (!t)
		return (-1);
	//printf("check t %s\n", t);
	dirp = opendir(t);
	if (!dirp)
	{
		if (access(path, F_OK) == 0)
			printf("minishell: cd : %s: Not a directory\n", path);
		else
			printf("minishell: cd : %s: No such file or directory\n", path);
		ft_free(t);
		return (1);
	}
	closedir(dirp);
	original_pwd = ft_getenv("PWD", *ep);
	if (chdir(t) != 0)
    {
		perror("chdir");
		ft_free(t);
		return (1);
	}
	cwd = getcwd(cwd, 0);
	do_export("OLDPWD", original_pwd, ep, NULL);
	do_export("PWD", cwd, ep, NULL);
	ft_free(cwd);
	if (f == 1)
		ft_free(t);
	return (0);
}

int	exec_cd(t_command *cmd, t_envp **ep)
{
	if (cmd->cmd_arg[1] == NULL)
		return (do_cd(NULL, ep));
	else if (cmd->cmd_arg[1] != NULL && cmd->cmd_arg[2] == NULL)
		return (do_cd(cmd->cmd_arg[1], ep));
	else if (cmd->cmd_arg[2] != NULL)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	return (0); //??
}


int	do_pwd(t_envp *ep)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(cwd, 0);
	if (!cwd)
	{
		cwd = ft_strdup(ft_getenv("PWD", ep));
		if (!cwd)
			return (-1); // a voir
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}