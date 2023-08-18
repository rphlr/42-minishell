/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:19:43 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/18 16:32:33 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 9223372036854775807 is the max value of a long long int (64 bits)
// -9223372036854775807 is the min value of a long long int (64 bits)
static bool	check_exit_token(char *arg)
{
	char *tmp;
	char *max_ll_str;
	char *min_ll_str;

	tmp = arg;
	max_ll_str = "9223372036854775807";
	min_ll_str = "-9223372036854775807";
	if (ft_strlen(arg) > 20)
		return (false);
	if (ft_strlen(arg) > ft_strlen(max_ll_str))
		return (false);
	else if (ft_strlen(arg) == ft_strlen(max_ll_str))
	{
		int i = 1;
		while (arg[i])
		{
			if (arg[i] > max_ll_str[i])
				return (false);
			i++;
		}
	}
	if (ft_strlen(arg) > ft_strlen(min_ll_str))
		return (false);
	else if (ft_strlen(arg) == ft_strlen(min_ll_str))
	{
		int i = 1;
		while (arg[i])
		{
			if (arg[i] > min_ll_str[i])
				return (false);
			i++;
		}
	}
	if (!*tmp)
		return (false);
	while (*tmp)
	{
		if (!ft_isdigit(*tmp) && *tmp != '-' && *tmp != '+')
			return (false);
		tmp++;
	}
	return (true);
}

void	ft_exit(t_global *global)
{
	int	i;

	i = 1;
	global->exit_code = 0;
	ft_printf("exit\n");
	if (global->line->token[i + 1] && !ft_strcmp(global->line->token[i + 1], "--"))
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
