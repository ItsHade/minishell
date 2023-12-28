
#include "../include/minishell.h"

extern int g_return;

int	ft_closepipe(int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
	return (0);
}

int	ft_open_here_doc(t_data *data, int i)
{
	char *file;
	char *nb;

	file = NULL;
	nb = ft_itoa(i);
	if (!nb)
		return (-1);
	file = ft_strjoin("/tmp/.here_doc", nb);
	free(nb);
	if (!file)
			return (-1);
	data->infile = open(file, O_RDONLY, 00644);
	free(file);
	if (data->infile < 0)
		return (ft_error(NULL, file), -1);
	if (dup2(data->infile, STDIN_FILENO) == -1)
		return (close(data->infile), -1);
	close(data->infile);
	return (g_return);
}

//int 	ft_open_files(t_data *data) // still need to figure out modes for >> (OAPPEND)
int	ft_open_file(t_data *data, char *file, int mode, int i)
{
	if (mode == I_RDR)
	{
		data->infile = open(file, O_RDONLY, 00644);
		if (data->infile < 0)
			return (ft_error(NULL, file), -1);
		if (dup2(data->infile, STDIN_FILENO) == -1)
			return (close(data->infile), -1);
		close(data->infile);
	}
	else if (mode == O_RDR)
	{
		data->outfile = open(file, O_WRONLY | O_CREAT | O_TRUNC, 00644);
		if (data->outfile < 0)
			return (ft_error(NULL, file), -1);
		if (dup2(data->outfile, STDOUT_FILENO) == -1)
			return (close(data->outfile), -1);
		close(data->outfile);
	}
	else if (mode == OAPPEND)
	{
		data->outfile = open(file, O_WRONLY | O_CREAT | O_APPEND, 00644);
		if (data->outfile < 0)
			return (ft_error(NULL, file), -1);
		if (dup2(data->outfile, STDOUT_FILENO) == -1)
			return (close(data->outfile), -1);
		close(data->outfile);
	}
	else if (mode == HEREDOC)
		return (ft_open_here_doc(data, i));
	return (g_return);
}

int	ft_io_file(t_data *data, t_envp **envp, int i, int is_pipe)
{
	int	a;

	a = 0;
	while (a < data->commands[i].nb_io)
	{
		if (data->commands[i].files[a] == NULL)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(data->commands[i].files[a], 2);
			ft_putstr_fd(": ambiguous redirect\n", 2);
			
		}
		if (ft_open_file(data, data->commands[i].files[a], data->commands[i].redir[a], i) == -1)
		{
			//
			ft_putstr_fd("FILE FAILED TO OPEN\n", 2);
			if (is_pipe == 1)
			{
				ft_freecmdtable(data);
				clear_ep(envp);
				g_return = 1;
				exit (g_return);
			}
			g_return = 1;
			return (g_return);
		}
		a++;
	}
	return (0);
}
