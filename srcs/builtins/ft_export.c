/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:53:18 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/30 16:26:22 by rrouille         ###   ########.fr       */
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

t_env	*clone_env_list(t_env *head)
{
	t_env	*new_head;
	t_env	*new_current;
	t_env	*current;
	t_env	*new_node;

	new_head = NULL;
	new_current = NULL;
	current = head;
	while (current)
	{
		new_node = malloc(sizeof(t_env));
		new_node->name = ft_strdup(current->name);
		new_node->value = ft_strdup(current->value);
		new_node->next = NULL;
		if (!new_head)
		{
			new_head = new_node;
			new_current = new_node;
		}
		else
		{
			new_current->next = new_node;
			new_current = new_node;
		}
		current = current->next;
	}
	return (new_head);
}

static void	sorting_env(t_env *env_cpy)
{
	t_env	*current;
	t_env	*tmp;
	char	*tmp_name;
	char	*tmp_value;

	current = env_cpy;
	while (current)
	{
		tmp = current->next;
		while (tmp)
		{
			if (ft_strcmp(current->name, tmp->name) > 0)
			{
				tmp_name = current->name;
				tmp_value = current->value;
				current->name = tmp->name;
				current->value = tmp->value;
				tmp->name = tmp_name;
				tmp->value = tmp_value;
			}
			tmp = tmp->next;
		}
		current = current->next;
	}
	current = env_cpy;
	while (current)
	{
		ft_printf("declare -x %s=\"%s\"\n", current->name, current->value);
		current = current->next;
	}
}

void	ft_export(t_global *global, t_line *line)
{
	char	*name;
	char	*value;
	t_env	*new_item;
	t_env	*current;
	t_env	*clone_env;
	int		i;

	clone_env = NULL;
	if (line->count->nbr_tokens < 2)
	{
		clone_env = clone_env_list(global->env);
		sorting_env(clone_env);
		return ;
	}
	name = NULL;
	value = NULL;
	i = 1;
	while (line->token[i])
	{
		if (!ft_isalpha(line->token[i][0]) && line->token[i][0] != '_')
		{
			ft_printf("minishell: export: `%s': not a valid identifier\n",
				line->token[i]);
			return ;
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
