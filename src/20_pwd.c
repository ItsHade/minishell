/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maburnet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 18:46:45 by maburnet          #+#    #+#             */
/*   Updated: 2023/12/30 18:46:47 by maburnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	do_pwd(t_envp *ep)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(cwd, 0);
	if (!cwd)
	{
		cwd = ft_strdup(ft_getenv("PWD", ep));
		if (!cwd)
			return (-1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
