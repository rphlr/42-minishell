/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 07:34:38 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/07 18:47:31 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ignore_quotes(const char *str)
{
	int		i;
	char	*result;

	i = 0;
	result = (char *)ft_gc_malloc((ft_strlen(str) + 1) * sizeof(char));
	if (!result)
		return (NULL);
	while (*str)
	{
		if (*str != '\'' && *str != '\"')
			result[i++] = *str;
		str++;
	}
	result[i] = '\0';
	return (result);
}

int	check_syntax(char *str)
{
	int	nbr_of_simple_cote;
	int	nbr_of_double_cote;

	nbr_of_simple_cote = 0;
	nbr_of_double_cote = 0;
	while (*str)
	{
		if (*str == '\'')
			nbr_of_simple_cote++;
		if (*str == '\"')
			nbr_of_double_cote++;
	}
	if (nbr_of_simple_cote % 2 != 0 || nbr_of_double_cote % 2 != 0)
		return (0);
	return (1);
}

void	echo_print_special_char(char *str)
{
	while (*str)
	{
		str = ignore_quotes(str);
		ft_printf("%c", *str++);
	}
}

void	ft_echo(t_cmd *cmd)
{
	int	arg;

	arg = 0;
	ft_printf("arg = %d\n", arg);
	if (arg == 1)
	{
		while (*++cmd->token)
		{
			*cmd->token = ignore_quotes(*cmd->token);
			ft_printf("%s", *cmd->token);
			if (*(cmd->token + 1))
				ft_printf(" ");
		}
	}
	else if (arg == 2)
	{
		while (*++cmd->token)
		{
			echo_print_special_char(ignore_quotes(*cmd->token));
			if (*(cmd->token + 1))
				ft_printf(" ");
		}
	}
	else
	{
		while (*++cmd->token)
		{
			*cmd->token = ignore_quotes(*cmd->token);
			ft_printf("%s", *cmd->token);
			if (*(cmd->token + 1))
				ft_printf(" ");
		}
		ft_printf("\n");
	}
}
