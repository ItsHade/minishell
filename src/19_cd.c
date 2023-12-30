/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maburnet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 13:26:17 by maburnet          #+#    #+#             */
/*   Updated: 2023/12/30 13:26:20 by maburnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_ab_path(char *path)
{
	char	*t;
	char	*filename;
	char	*cwd;

	cwd = NULL;
	t = NULL;
	filename = NULL;
	cwd = getcwd(cwd, 0);
	if (path[0] == '.' && path[1] == '/')
		filename = ft_substr(path, 1, ft_strlen(path) - 1);
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

int	ft_get_path(char *path, t_envp **ep, char **t, int *f)
{
	if (path == NULL || (path[0] == '~' && (!path[1] || path[1] == '/')))
	{
		*t = ft_getenv("HOME", *ep);
		*f = 0;
		if (!(*t))
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	}
	else if ((path[0] == '.' && path[1] == '/')
		|| (path[0] != '.' && path[0] != '/'))
		*t = get_ab_path(path);
	else
		*t = ft_strdup(path);
	if (!(*t))
		return (-1);
	return (0);
}

int	ft_open_dir(DIR **dirp, char *t, char *path)
{
	*dirp = opendir(t);
	if (!(*dirp))
	{
		if (access(path, F_OK) == 0)
			printf("minishell: cd : %s: Not a directory\n", path);
		else
			printf("minishell: cd : %s: No such file or directory\n", path);
		return (1);
	}
	return (0);
}

int	do_cd(char *path, t_envp **ep)
{
	DIR		*dirp;
	char	*t;
	char	*original_pwd;
	char	*cwd;
	int		f;

	cwd = NULL;
	f = 1;
	original_pwd = NULL;
	t = NULL;
	if (ft_get_path(path, ep, &t, &f) == -1)
		return (-1);
	if (ft_open_dir(&dirp, t, path) == 1)
		return (free(t), 1);
	closedir(dirp);
	original_pwd = ft_getenv("PWD", *ep);
	if (chdir(t) != 0)
		return (perror("chdir"), ft_free(t), 1);
	cwd = getcwd(cwd, 0);
	do_export("OLDPWD", original_pwd, ep, NULL);
	do_export("PWD", cwd, ep, NULL);
	if (f == 1)
		ft_free(t);
	return (ft_free(cwd), 0);
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
	return (0);
}
