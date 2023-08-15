/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 12:42:00 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/15 17:12:15 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(char *name, t_global *global)
{
	t_env	*tmp;

	tmp = global->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, name) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

void	ft_env(t_global *global)
{
	t_env	*tmp;

	tmp = global->env;
	while (tmp)
	{
		ft_printf("%s=%s\n", tmp->name, tmp->value);
		tmp = tmp->next;
	}
}
