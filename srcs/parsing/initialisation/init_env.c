/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 16:18:59 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/28 16:20:44 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*new_env_item(char *env_str)
{
	t_env	*new_item;

	new_item = ft_gc_malloc(sizeof(t_env));
	if (!new_item)
		return (NULL);
	if (ft_strchr(env_str, '='))
	{
		new_item->name = ft_strndup(env_str, ft_strchr(env_str, '=') - env_str);
		new_item->value = ft_strdup(ft_strchr(env_str, '=') + 1);
	}
	new_item->next = NULL;
	return (new_item);
}

static void	add_to_list(t_env **head, t_env **current, t_env *new_item)
{
	if (!*head)
	{
		*head = new_item;
		*current = *head;
	}
	else
	{
		(*current)->next = new_item;
		*current = new_item;
	}
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*current;
	t_env	*new_item;

	head = NULL;
	current = NULL;
	while (*envp)
	{
		new_item = new_env_item(*envp);
		if (!new_item)
			return (NULL);
		add_to_list(&head, &current, new_item);
		envp++;
	}
	return (head);
}
