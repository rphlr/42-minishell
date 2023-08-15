/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:49:26 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/15 17:15:24 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**config_cmds(char **tokens, t_token *type)
{
	int		i;
	int		j;
	char	**cmd;

	i = -1;
	j = 0;
	cmd = ft_gc_malloc(sizeof(char *) * (ft_tablen(tokens) + 1));
	if (!cmd)
		return (NULL);
	while (type[++i] != END || type[i] == PIPE)
	{
		if (type[i] != END && type[i] != PIPE)
		{
			if (!i || type[i - 1] == PIPE)
				cmd[j++] = ft_strdup(tokens[i]);
			else
			{
				cmd[j - 1] = ft_strjoin(cmd[j - 1], " ");
				cmd[j - 1] = ft_strjoin(cmd[j - 1], tokens[i]);
			}
		}
	}
	if (j == 0)
		cmd = NULL;
	return (cmd);
}

t_cmd	*init_cmds(char **tokens)
{
	t_cmd	*cmd;
	t_state	error_state;

	cmd = ft_gc_malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	error_state = VALID;
	cmd->token = tokens;
	cmd->type = init_tokens_type(tokens);
	error_state = check_errors(cmd->type, tokens);
	if (error_state)
		return (NULL);
	cmd->cmd = config_cmds(tokens, cmd->type);
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->pipe = NULL;
	cmd->heredoc = NULL;
	cmd->nbr_cmd = count_cmd(cmd->type);
	cmd->nbr_token = ft_tablen(tokens);
	cmd->nbr_pipe = count_pipe(cmd->type);
	cmd->nbr_redirection = count_redirection(cmd->type);
	cmd->next = NULL;
	return (cmd);
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
		new_item = ft_gc_malloc(sizeof(t_env));
		if (!new_item)
			return (NULL);
		if (ft_strchr(*envp, '='))
		{
			new_item->name = ft_strndup(*envp, ft_strchr(*envp, '=') - *envp);
			new_item->value = ft_strdup(ft_strchr(*envp, '=') + 1);
			new_item->is_env = false;
		}
		new_item->next = NULL;
		if (!head)
		{
			head = new_item;
			current = head;
		}
		else
		{
			current->next = new_item;
			current = new_item;
		}
		envp++;
	}
	return (head);
}

t_global	*init_global(char **envp)
{
	t_global	*global;

	global = ft_gc_malloc(sizeof(t_global));
	if (!global)
		return (NULL);
	global->exit_code = 0;
	global->pid = getpid();
	global->env = init_env(envp);
	if (!global->env)
		return (NULL);
	global->cmd = NULL;
	return (global);
}
