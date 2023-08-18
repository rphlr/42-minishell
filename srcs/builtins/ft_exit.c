/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:19:43 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/18 14:49:29 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	check_exit_token(char *arg)
{
	if (!*arg)
		return (false);
	while (*arg)
	{
		if (!ft_isdigit(*arg) && *arg != '-' && *arg != '+')
			return (false);
		arg++;
	}
	return (true);
}

void	ft_exit(t_global *global)
{
	int	i;

	i = 1;
	global->exit_code = 0;
	ft_printf("exit\n");
	if (!ft_strcmp(*(global->line->token + 1), "--"))
		i++;
	if (!global->line->token[i])
		exit (global->exit_code);
	else if (!check_exit_token(global->line->token[i]))
	{
		ft_printf("minishell: exit: %s: numeric argument required\n",
			global->line->token[i]);
		global->exit_code = 255;
	}
	else if (global->line->token[i + 1])
	{
		ft_printf("minishell: exit: too many arguments\n");
		return ;
	}
	else
	{
		global->exit_code = ft_atoi(global->line->token[i]);
		if (global->exit_code < 0 || global->exit_code > 255)
			global->exit_code %= 256;
	}
	exit(global->exit_code);
}
