/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:38:18 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/18 23:52:12 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	count_cmd(t_token *type)
// {
// 	int	nbr_cmd;

// 	nbr_cmd = 0;
// 	while (*type != END)
// 	{
// 		if (*type++ == PIPE)
// 			nbr_cmd++;
// 	}
// 	return (nbr_cmd + 1);
// }

// int	count_semicolon(t_token *type)
// {
// 	int	nbr_semicolon;

// 	nbr_semicolon = 0;
// 	while (*type != END)
// 	{
// 		if (*type++ == SEMICOLON)
// 			nbr_semicolon++;
// 	}
// 	return (nbr_semicolon);
// }

// int	count_and(t_token *type)
// {
// 	int	nbr_and;

// 	nbr_and = 0;
// 	while (*type != END)
// 	{
// 		if (*type++ == AND)
// 			nbr_and++;
// 	}
// 	return (nbr_and);
// }

// int	count_or(t_token *type)
// {
// 	int	nbr_or;

// 	nbr_or = 0;
// 	while (*type != END)
// 	{
// 		if (*type++ == OR)
// 			nbr_or++;
// 	}
// 	return (nbr_or);
// }

// int	count_pipe(t_token *type)
// {
// 	int	nbr_pipe;

// 	nbr_pipe = 0;
// 	while (*type != END)
// 	{
// 		if (*type++ == PIPE)
// 			nbr_pipe++;
// 	}
// 	return (nbr_pipe);
// }

// int	count_redirection(t_token *type)
// {
// 	int	nbr_redirection;

// 	nbr_redirection = 0;
// 	while (*type != END)
// 	{
// 		if (*type == INPUT || *type == OUTPUT || *type == APPEND
// 			|| *type == HEREDOC)
// 			nbr_redirection++;
// 		type++;
// 	}
// 	return (nbr_redirection);
// }

t_count	*count_types(t_token *type)
{
	t_count	*count;

	count = ft_gc_malloc(sizeof(t_count));
	if (!count)
		return (NULL);
	while (*type != END)
	{
		if (*type == INPUT)
			count->nbr_inputs++;
		else if (*type == OUTPUT)
			count->nbr_outputs++;
		else if (*type == APPEND)
			count->nbr_appends++;
		else if (*type == HEREDOC)
			count->nbr_heredocs++;
		else if (*type == PIPE)
			count->nbr_pipes++;
		else if (*type == SEMICOLON)
			count->nbr_semicolons++;
		else if (*type == AND)
			count->nbr_ands++;
		else if (*type == OR)
			count->nbr_ors++;
		else if (*type == OPTIONS)
			count->nbr_options++;
		type++;
	}
	return (count);
}
