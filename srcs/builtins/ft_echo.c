/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 07:34:38 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/22 14:21:54 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_option(char *str)
{
	while (*++str)
	{
		if (*str != 'n')
			return (0);
	}
	return (1);
}

static void	echo_print(char *str, t_global *global, int i)
{
	char	*output;

	output = ft_gc_malloc(ft_strlen(str) + 1);
	if (!output)
		return ;
	if (global->line->type[i] != CLOSED_DQUOTE && global->line->type[i] != CLOSED_QUOTE)
	{
		while (*str)
		{
			if (*str == '\\')
			{
				str++;
				ft_printf("%c", *str);
				str++;
				continue ;
			}
			ft_printf("%c", *str++);
		}
		return ;
	}
	while (*str)
		ft_printf("%c", *str++);
}

void	ft_echo(t_global *global)
{
	int	nwln;
	int	i;

	nwln  = 1;
	i = 0;
	global->line->token++;
	global->line->type++;
	while (global->line->token[i] && global->line->type[i] == OPTIONS)
	{
		if (check_option(global->line->token[i]))
		{
			nwln = 0;
			i++;
		}
		else
			break ;
	}
	while (global->line->token[i])
	{
		echo_print(global->line->token[i], global, i);
		i++;
		if (global->line->token[i])
			ft_printf(" ");
	}
	if (nwln)
		ft_printf("\n");
}
