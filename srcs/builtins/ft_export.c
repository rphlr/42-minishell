/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:53:18 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/13 11:05:48 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void extract_name_value(char *token, char **name, char **value)
{
	*name = strtok(token, "=");
	*value = strtok(NULL, "=");
}

static t_env *new_env_item(char *name, char *value)
{
	t_env *new_item = (t_env *)ft_gc_malloc(sizeof(t_env));
	if (!new_item)
		return (NULL);
	new_item->name = strdup(name);
	new_item->value = strdup(value);
	new_item->is_env = true;
	new_item->next = NULL;
	return (new_item);
}

void	ft_export(t_global *global, t_cmd *cmd)
{
	if (cmd->nbr_token < 2)
	{
		// Handle error: Not enough tokens for export command
		return;
	}

	char *name = NULL;
	char *value = NULL;
	extract_name_value(cmd->token[1], &name, &value);

	if (!name || !value)
	{
		// Handle error: Invalid export format
		return;
	}

	t_env *new_item = new_env_item(name, value);
	if (!new_item)
	{
		// Handle error: Memory allocation failed
		return;
	}

	// Add the new item to the end of the env list
	if (!global->env)
	{
		global->env = new_item;
	}
	else
	{
		t_env *current = global->env;
		while (current->next)
			current = current->next;
		current->next = new_item;
	}
}
