/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:53:18 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/16 13:21:32 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	extract_name_value(char *token, char **name, char **value)
{
	*name = ft_strtok(token, "=");
	*value = ft_strtok(NULL, "=");
	if (!*name || !*value)
	{
		*name = token;
		*value = "";
	}
	else if (ft_strchr(*name, '='))
	{
		*name = NULL;
		*value = NULL;
	}
}

static t_env	*new_env_item(char *name, char *value)
{
	t_env	*new_item;

	new_item = (t_env *)ft_gc_malloc(sizeof(t_env));
	if (!new_item)
		return (NULL);
	new_item->name = ft_strdup(name);
	new_item->value = ft_strdup(value);
	new_item->next = NULL;
	return (new_item);
}

void	ft_export(t_global *global, t_line *line)
{
	char	*name;
	char	*value;
	t_env	*new_item;
	t_env	*current;
	int		i;

	if (line->nbr_token < 2)
	{
		ft_env(global);
		return ;
	}
	name = NULL;
	value = NULL;
	i = 1;
	while (line->token[i])
	{
		if (line->type[i] == OPTIONS)
		{
			if (!ft_strcmp(line->token[i], "-"))
				ft_env(global);
			else
				ft_printf("minishell: export: options not implemented\n");
			break ;
		}
		extract_name_value(line->token[i], &name, &value);
		if (!name || !value)
		{
			ft_printf("minishell: export: bad assignment\n");
			return ;
		}
		current = global->env;
		while (current)
		{
			if (!ft_strcmp(current->name, name))
			{
				current->value = ft_strdup(value);
				return ;
			}
			current = current->next;
		}
		new_item = new_env_item(name, value);
		if (!new_item)
			return ;
		if (!global->env)
			global->env = new_item;
		else
		{
			current = global->env;
			while (current->next)
				current = current->next;
			current->next = new_item;
		}
		i++;
	}
}
