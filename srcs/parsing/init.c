/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:49:26 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/16 14:43:29 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmds	*init_cmds(char **tokens, t_token *type)
{
	t_cmds	*head;
	t_cmds	*current;
	t_cmds	*new_cmd;
	char	*full_cmd;
	char	*temp;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while (tokens[i] != NULL)
	{
		full_cmd = ft_strdup(tokens[i]);
		while (tokens[i + 1] != NULL && type[i + 1] != PIPE)
		{
			i++;
			temp = full_cmd;
			full_cmd = ft_strjoin(temp, tokens[i]);
			if (tokens[i + 1] != NULL && type[i + 1] != PIPE)
				full_cmd = ft_strjoin(full_cmd, " ");
		}
		new_cmd = (t_cmds *)ft_gc_malloc(sizeof(t_cmds));
		if (!new_cmd)
			return (NULL);
		new_cmd->cmd = full_cmd;
		new_cmd->input = NULL;
		new_cmd->output = NULL;
		new_cmd->next = NULL;
		if (head == NULL)
		{
			head = new_cmd;
			current = head;
		}
		else
		{
			current->next = new_cmd;
			current = new_cmd;
		}
		i++;
		while (tokens[i] != NULL && type[i] != PIPE)
		{
			if (type[i] == INPUT)
			{
				new_cmd->input = (t_redirection *)ft_gc_malloc(sizeof(t_redirection));
				if (!new_cmd->input)
					return (NULL);
				new_cmd->input->type = INPUT_REDIRECTION;
				new_cmd->input->filename = ft_strdup(tokens[i + 1]);
				i += 2;
			}
			else if (type[i] == OUTPUT)
			{
				new_cmd->output = (t_redirection *)ft_gc_malloc(sizeof(t_redirection));
				if (!new_cmd->output)
					return (NULL);
				new_cmd->output->type = OUTPUT_REDIRECTION;
				new_cmd->output->filename = ft_strdup(tokens[i + 1]);
				i += 2;
			}
			else if (type[i] == APPEND)
			{
				new_cmd->output = (t_redirection *)ft_gc_malloc(sizeof(t_redirection));
				if (!new_cmd->output)
					return (NULL);
				new_cmd->output->type = APPEND_REDIRECTION;
				new_cmd->output->filename = ft_strdup(tokens[i + 1]);
				i += 2;
			}
			else
				i++;
		}
		if (tokens[i] != NULL && type[i] == PIPE)
			i++;
	}
	return (head);
}

t_line	*init_line(char **tokens)
{
	t_line	*line;
	t_state	error_state;

	line = ft_gc_malloc(sizeof(t_line));
	if (!line)
		return (NULL);
	error_state = VALID;
	line->token = tokens;
	line->type = init_tokens_type(tokens);
	error_state = check_errors(line->type, tokens);
	if (error_state)
		return (NULL);
	line->cmds = init_cmds(tokens, line->type);
	line->pipe = NULL;
	line->heredoc = NULL;
	line->nbr_cmd = count_cmd(line->type);
	line->nbr_token = ft_tablen(tokens);
	line->nbr_pipe = count_pipe(line->type);
	line->nbr_redirection = count_redirection(line->type);
	return (line);
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
	global->env = init_env(envp);
	if (!global->env)
		return (NULL);
	global->line = NULL;
	return (global);
}
