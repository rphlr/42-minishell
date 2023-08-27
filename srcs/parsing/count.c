/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:38:18 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/28 00:14:07 by mariavillar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_count	*count_types(t_token *type)
{
	t_count	*count;

	count = ft_gc_malloc(sizeof(t_count));
	if (!count)
		return (NULL);
	count->special_cases = false;
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
		{
			type++;
			count->nbr_options++;
			continue;
		}
		else
		{
			type++;
			count->nbr_tokens++;
			continue;
		}
		count->special_cases = true;
		count->nbr_tokens++;
		type++;
	}
	count->nbr_cmds = count->nbr_pipes + 1;
	return (count);
}
