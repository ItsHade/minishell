#include "../include/minishell.h"

extern int g_return;


int	ft_expand_cmd(t_data *data, t_envp **env, int i) //echo \n  OU  << outfile
{
	t_exp *exp;
	int	a;

	a = 0;
	exp = NULL;
	if (data->commands[i].cmd_arg == NULL)
		return (0);
	//data->commands[i].is_null = ft_calloc(sizeof(int), (data->commands[i].nb_arg + 1));
	while (data->commands[i].cmd_arg[a] != NULL)
	{
		if (do_quote_exp(&exp, data->commands[i].cmd_arg[a], *env, 1) == -1)
			return (-1); //""   ''    $NO
		// if (!exp)
		// {
		// 	if (data->commands[i].cmd_arg[a][0] == '$')
		// 		data->commands[i].is_null[a] = 1;
		// }
		free(data->commands[i].cmd_arg[a]);
		if (!exp)
			data->commands[i].cmd_arg[a] = ft_strdup("");
		else
			data->commands[i].cmd_arg[a] = exp_to_str(exp);
		clear_exp(&exp);
		exp = NULL;
		if (!data->commands[i].cmd_arg[a])
			return (-1);
		a++;
	}
	return (0);
}

int	is_amb_cmd(char *t, t_envp *ep)
{
	int	dl;
	int	ret;

	dl = 0;
	while (t[dl] && t[dl] != '$')
		dl++;
	if (t[dl] != '$')
	{
		ft_free(t);
		return (0);
	}
	if (add_dollar_str(&t, dl, ep) == -1)
		return (-1);
	ret = check_amb_str(t);
	ft_free(t);
	return (ret);
}


int	ft_expand_files(t_data *data, t_envp **env, int i)
{
	t_exp *exp;
	int	a;
	int	is_amb;

	a = 0;
	exp = NULL;
	if (data->commands[i].files == NULL)
		return (0);
	while (data->commands[i].files[a] != NULL)
	{
		//si ambigue
		is_amb = is_amb_cmd(ft_strdup(data->commands[i].files[a]), *env);
		//printf("%s++++%d\n", data->commands[i].files[a], is_amb);
		if (is_amb == -1)
			return (-1);
		else if (is_amb == 1)
		{
			a++;
			continue;
		}
		if (do_quote_exp(&exp, data->commands[i].files[a], *env, 1) == -1)
			return (-1);
		free(data->commands[i].files[a]);
		if (!exp)
			data->commands[i].files[a] = ft_strdup("");
		else
			data->commands[i].files[a] = exp_to_str(exp);
		clear_exp(&exp);
		exp = NULL;
		if (!data->commands[i].files[a])
			return (-1);
		a++;
	}
	return (0);
}

//ok$RIEN

// ls > $RIEN
// ls > '$RIEN'
// data->commands[i]->is_ambiguous;

// $RIEN --> nom du fichier a creer

// $RIEN --> ambiguous __REDIRECT