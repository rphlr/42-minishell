/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 12:41:30 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/13 14:11:30 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void remove_env_item(t_env **head, char *name)
{
    t_env *current = *head;
    t_env *prev = NULL;

    while (current && ft_strcmp(current->name, name) != 0)
    {
        prev = current;
        current = current->next;
    }
    if (!current)
        return;
    if (!prev)
        *head = current->next;
    else
        prev->next = current->next;
}

void	ft_unset(t_global *global, t_cmd *cmd)
{
	int i;

	i = 1;
    if (cmd->nbr_token < 2)
    {
		ft_printf("minishell: unset: not enough arguments\n");
        return;
    }
	else
	{
		while (cmd->token[i])
		{
			char *name_to_remove = cmd->token[i++];
			remove_env_item(&global->env, name_to_remove);
		}
	}
}

