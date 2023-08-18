/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 07:34:38 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/18 23:17:22 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_option(char *str)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static void	echo_print(char *str, t_global *global)
{
	int		in_single_quote;
	int		in_double_quote;
	char	*end;
	char	temp;
	char	*var_value;

	in_single_quote = 0;
	in_double_quote = 0;
	while (*str)
	{
		if (*str == '\\' && !in_single_quote)
		{
			str++;
			if (in_double_quote)
			{
				if (*str == 'n')
					ft_printf("\n");
				else if (*str == 't')
					ft_printf("\t");
				else if (*str == 'v')
					ft_printf("\v");
				else if (*str == 'b')
					ft_printf("\b");
				else if (*str == 'r')
					ft_printf("\r");
				else if (*str == 'f')
					ft_printf("\f");
				else if (*str == 'a')
					ft_printf("\a");
				else if (*str == '0')
					ft_printf("\0");
				else if (*str == '\\')
					ft_printf("\\");
				else
					ft_printf("\\%c", *str);
			}
			else
				ft_printf("%c", *str);
			str++;
			continue ;
		}
		if (*str == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			str++;
			continue ;
		}
		else if (*str == '\"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			str++;
			continue ;
		}
		if (in_single_quote)
		{
			ft_printf("%c", *str++);
			continue ;
		}
		if (*str == '$' && !in_single_quote)
		{
			str++;
			if (*str == '?')
				ft_printf("%d", global->exit_code);
			else if ((*str >= 'a' && *str <= 'z') || (*str >= 'A'
					&& *str <= 'Z') || (*str >= '0' && *str <= '9')
				|| *str == '_')
			{
				end = str;
				while (*end && ((*end >= 'a' && *end <= 'z') || (*end >= 'A'
							&& *end <= 'Z') || (*end >= '0' && *end <= '9')
						|| *end == '_'))
					end++;
				temp = *end;
				*end = '\0';
				var_value = get_env_value(str, global);
				if (var_value)
					ft_printf("%s", var_value);
				else
					ft_printf("");
				*end = temp;
				str = end;
			}
			else
				ft_printf("");
			str++;
		}
		else
			ft_printf("%c", *str++);
	}
}

void	ft_echo(t_global *global)
{
	int	newline;

	newline = 1;
	global->line->token++;
	global->line->type++;
	while (global->line->token && *global->line->type++ == OPTIONS)
	{
		if (check_option(*global->line->token))
		{
			newline = 0;
			global->line->token++;
		}
		else
			break ;
	}
	while (*global->line->token)
	{
		echo_print(*global->line->token, global);
		global->line->token++;
		if (*global->line->token)
			ft_printf(" ");
	}
	if (newline)
		ft_printf("\n");
}
