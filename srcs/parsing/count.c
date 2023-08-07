/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:38:18 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/07 16:20:43 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_tablen(char **tab)
{
	int	len;

	len = 0;
	while (*tab++)
		len++;
	return (len);
}

int	count_cmd(t_token *type)
{
	int	nbr_cmd;

	nbr_cmd = 0;
	while (*type != END)
	{
		if (*type++ == PIPE)
			nbr_cmd++;
	}
	return (nbr_cmd + 1);
}

int	count_pipe(t_token *type)
{
	int	nbr_pipe;

	nbr_pipe = 0;
	while (*type != END)
	{
		if (*type++ == PIPE)
			nbr_pipe++;
	}
	return (nbr_pipe);
}

int	count_redirection(t_token *type)
{
	int	nbr_redirection;

	nbr_redirection = 0;
	while (*type != END)
	{
		if (*type == INPUT || *type == OUTPUT || *type == APPEND
			|| *type == HEREDOC)
			nbr_redirection++;
		type++;
	}
	return (nbr_redirection);
}
