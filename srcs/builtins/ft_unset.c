/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 12:41:30 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/13 11:11:38 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void remove_env_item(t_env **head, char *name)
{
    t_env *current = *head;
    t_env *prev = NULL;

    while (current != NULL && strcmp(current->name, name) != 0)
    {
        prev = current;
        current = current->next;
    }

    if (current == NULL) // L'élément n'a pas été trouvé
        return;

    if (prev == NULL) // L'élément à supprimer est le premier élément
    {
        *head = current->next;
    }
    else
    {
        prev->next = current->next;
    }

    // free(current->name);
    // free(current->value);
    // free(current);
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

