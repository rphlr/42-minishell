/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:19:43 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/17 15:48:41 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	check_exit_token(char *arg)
{
	if (!ft_isdigit(ft_atoi(arg)))
	{
		if (ft_strcmp(arg, "+") || ft_strcmp(arg, "--"))
			return (true);
		ft_printf("minishell: exit: %s: numeric argument required\n", arg);
		return (false);
	}
	return (true);
}

void	ft_exit(t_global *global)
{
	int	i;

	i = 1;
	global->exit_code = 0;
	ft_printf("exit\n");
	if (ft_strcmp(*global->line->token, "--"))
		i++;
	if (!global->line->token[i])
		exit (global->exit_code);
	else if (!check_exit_token(global->line->token[i]))
		global->exit_code = 255;
	else if (global->line->token[i + 1])
	{
		ft_printf("minishell: exit: too many arguments\n");
		return ;
	}
	else if (global->line->token[i])
	{
		// if (!ft_strcmp(global->line->token[1], "--"))
		// {
		// 	if (global->line->token[i + 1])
		// 	{
		// 		i++;
		// 		if (!check_exit_token(global->line->token[i]))
		// 			global->exit_code = 255;
		// 	}
		// }
		global->exit_code = ft_atoi(global->line->token[i]);
		if (global->exit_code < 0 || global->exit_code > 255)
			global->exit_code %= 256;
	}
	exit (global->exit_code);
}
