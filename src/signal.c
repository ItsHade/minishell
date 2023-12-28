#include "../include/minishell.h"

extern int g_return;

//not neededs
void	ft_child_signal(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("^C\n", 2);
		g_return = 130;
		exit(g_return);
	}

}

void	ft_here_doc_signal(int signum)
{
	if (signum == SIGINT)
	{
		close(STDIN_FILENO);
		g_return = -1; //dangereux probablement
	}

}

void	ft_signal(int signum)
{
	if (signum == SIGINT)
	{
		g_return = 130;
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}