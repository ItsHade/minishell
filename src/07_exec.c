
#include "../include/minishell.h"

extern int g_return;

int    ft_exec(char **command, t_envp **envp)
{
	char    *path;
	char	**env;

	if (!command[0])
		return (0);
	env = ft_lst_to_tab(envp);
	if (!env)
		return (g_return);
	if (!command)
		return (ft_freetab(env), g_return);
	if (ft_is_absolute(command[0]) == 0)
		return (ft_exec_abs(command, env));\
	else if (ft_is_absolute(command[0]) == 2)
		return (ft_error_msg(command[0], PERM, 126), ft_freetab(env), g_return);
	path = ft_findcmdpath(command[0], env, NULL, NULL);
	if (!path)
		return (ft_error_msg(command[0], NOTFOUND, 127), ft_freetab(env), g_return);
	if (execve(path, command, env) == -1)
	{
		fprintf(stderr, "EXECVE returned -1\n");
		if (errno == EACCES) //pour quels cas ? 
			ft_error_msg(command[0], NOTFOUND, 127); //changed from 126 to 127 for ""
		else
		{
			fprintf(stderr, "errno is not EACCESS\n");
			ft_error(NULL, command[0]);
			g_return = 127;
		}
		return (ft_free(path), ft_freetab(env), g_return);
	}
	return (0);
}

int	ft_do_child(t_data *data, t_envp **envp, int i)
{
	signal(SIGINT, ft_child_signal);
	signal(SIGQUIT, SIG_DFL);
	if (i != data->nb_cmd - 1)
	{
		if (dup2(data->pipefd[1], STDOUT_FILENO) == -1)
			return (ft_closepipe(data->pipefd),  -1);
	}
	close(data->pipefd[0]);
	close(data->pipefd[1]);
	g_return = ft_io_file(data, envp, i, 1);
	// if (data->commands[i].is_null[0] == 0)
	// {
	// 	g_return = ft_exec(data->commands[i].cmd_arg, envp);
			
	// }
	g_return = ft_exec(data->commands[i].cmd_arg, envp);
	ft_freecmdtable(data);
	clear_ep(envp);
	return (g_return);
}

int	ft_do_parent(t_data *data, int i, pid_t pid)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (i == (data->nb_cmd - 1))
	{
		data->pid = pid;
		close(STDOUT_FILENO);
		close(STDIN_FILENO);
	}
	else
	{
		if (dup2(data->pipefd[0], STDIN_FILENO) == -1)
			return (ft_closepipe(data->pipefd), -1);
	}
	close(data->pipefd[1]);
	close(data->pipefd[0]);
	return (0);
}

int ft_do_cmd(t_data *data, t_envp **envp, int i)
{
	pid_t	pid;

	if (pipe(data->pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (ft_closepipe(data->pipefd), perror("fork"), -1);
	if (pid == 0)
	{
		ft_close_std(data);
		g_return = ft_do_child(data, envp, i);
		exit(g_return);
	}
	else
		ft_do_parent(data, i, pid);
	return (0);
}

