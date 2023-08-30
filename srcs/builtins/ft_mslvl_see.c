/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mslvl_see.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 11:53:00 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/30 11:56:56 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_mslvl_see(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (!ft_strcmp(current->name, "MSLVL"))
		{
			if (!ft_strcmp(current->value, "1"))
				ft_printf("You are currently using %s level of our minishell.\n", current->value);
			else
				ft_printf("You are currently using %s levels of our minishell.\n", current->value);
			return ;
		}
		current = current->next;
	}
}