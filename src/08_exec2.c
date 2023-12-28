
#include "../include/minishell.h"

extern int g_return;

void	ft_exec_abs(char **command, char **envp)
{
	if (execve(command[0], command, envp) == -1)
	{
		if (command[0][0] == '/')
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(command[0], 2);
			ft_putstr_fd(": Is a directory\n", 2);
			g_return = 126;
			ft_freetab(envp);
			return ;
		}
		g_return = 127;
		ft_putstr_fd("minishell: ", 2);
		ft_cmd_not_found(command[0], 127);
		ft_freetab(envp);
	}
}

int	ft_is_absolute(char *cmd)
{
	struct stat fileStat;

    if (stat(cmd, &fileStat) == 0)
	{
		if (fileStat.st_mode & S_IFDIR)
			return (0);
	}
	return (-1);
}

int	ft_get_return_value(t_data *data)
{
	int	return_pid;
	int	i;

	i = -1;
	while (++i < data->nb_cmd)
	{
		return_pid = waitpid(0, &data->status, 0);
		if (return_pid == -1)
			return (-1);
		if (return_pid == data->pid && data->pid > 0)
		{
			if (data->status == 2)
			{
				ft_putstr_fd("\n", 2);
				g_return = 130;
			}
			else if (data->status >= 256)
				g_return = (data->status / 256);
			else
				g_return = (data->status % 256 + 256) % 256;
		}
	}
	if (g_return == 131)
		ft_putstr_fd("Quit (core dumped)\n", 2);
	return (0);
}

int	ft_exec_cmd(t_data *data, t_envp **envp, int i)
{
	while (i < data->nb_cmd)
	{
		ft_expand_cmd(data, envp, i);
		ft_expand_files(data, envp, i);
		if (ft_is_builtin(data, i) == 1)
		{
			if (ft_exec_builtin(data, envp, i) == -1)
				return (-1);
		}
		else
		{
			if (ft_do_cmd(data,  envp, i) == -1)
				return (-1);
		}
		i++;
	}
	ft_get_return_value(data);
	i = -1;
	while (++i < data->nb_cmd)
		ft_unlink_here_doc(data, i);
	return (0);
}

int	ft_execute(t_data *data, t_envp **envp) //??pkoi faut dup
{
	int	i;

	i = 0;
	data->status = 0;
	data->pid = -1;
	data->stdin = dup(STDIN_FILENO);
	data->stdout = dup(STDOUT_FILENO);
	if (ft_check_here_doc(data) == -1)
	{
		if (dup2(data->stdin, STDIN_FILENO) == -1) //??repeter
			return (ft_close_std(data), -1);
		return (ft_close_std(data), -1);
	}
	if (ft_exec_cmd(data, envp, i) == -1)
		return (ft_close_std(data), -1);
	if (dup2(data->stdin, STDIN_FILENO) == -1)
		return (perror("dup2"), ft_close_std(data), -1);
	if (dup2(data->stdout, STDOUT_FILENO) == -1)
		return (perror("dup2"), ft_close_std(data), -1);
    ft_close_std(data);
	return (0);
}
