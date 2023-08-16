/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:19:43 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/16 10:32:28 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	check_exit_token(char *arg)
{
	while (*arg)
	{
		if (!ft_isdigit(*arg))
		{
			ft_printf("minishell: exit: %s: numeric argument required\n", *arg);
			return (false);
		}
		arg++;
	}
	return (true);
}

void	ft_exit(t_global *global)
{
	global->exit_code = 0;
	ft_printf("exit\n");
	if (!global->line->token[1])
		exit(global->exit_code);
	if (!check_exit_token(global->line->token[1]))
		global->exit_code = 255;
	else if (global->line->token[2])
	{
		ft_printf("minishell: exit: too many arguments\n");
		return ;
	}
	else if (global->line->token[1])
	{
		global->exit_code = ft_atoi(global->line->token[1]);
		if (global->exit_code < 0 || global->exit_code > 255)
			global->exit_code %= 256;
	}
	exit(global->exit_code);
}
