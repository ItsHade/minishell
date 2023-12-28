/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maburnet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/15 17:00:34 by yzhuang           #+#    #+#             */
/*   Updated: 2023/12/16 14:50:27 by maburnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void    ft_putstr(char *str)
{

    if (!str)
    {
        write (1, "(null)", 6);
        return ;
    }
    write (1, str, ft_strlen(str));
}