/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:49:26 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/26 20:06:05 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    set_filename(t_cmds *head, char **tokens, t_token *type, int *i)
{
    while (tokens[*i] && type[*i] != PIPE  && type[*i] != SEMICOLON && type[*i] != AND && type[*i] != OR)
    {
        if (type[*i] == HEREDOC)
        {
            head->redir = (t_redirection *)ft_gc_malloc(sizeof(t_redirection));
            if (!head->redir)
                return ;
            head->redir->type = HEREDOC_REDIRECTION;
            head->redir->limiter = ft_strdup(tokens[*i + 1]);
            if (tokens[*i + 3] && (type[*i + 2] == INPUT || type[*i + 2] == OUTPUT || type[*i + 2] == APPEND))
            {
                head->redir->type_hd = type[*i + 2];
                head->redir->filename = ft_strdup(tokens[*i + 3]);
            }
            *i += 2;
            return ;
        }
        else if (type[*i] == INPUT)
        {
            head->redir = (t_redirection *)ft_gc_malloc(sizeof(t_redirection));
            if (!head->redir)
                return ;
            head->redir->type = INPUT_REDIRECTION;
            head->redir->filename = ft_strdup(tokens[*i + 1]);
            *i += 2;
        }
        else if (type[*i] == OUTPUT)
        {
            head->redir = (t_redirection *)ft_gc_malloc(sizeof(t_redirection));
            if (!head->redir)
                return ;
            head->redir->type = OUTPUT_REDIRECTION;
            head->redir->filename = ft_strdup(tokens[*i + 1]);
            *i += 2;
        }
        else if (type[*i] == APPEND)
        {
            head->redir = (t_redirection *)ft_gc_malloc(sizeof(t_redirection));
            if (!head->redir)
                return ;
            head->redir->type = APPEND_REDIRECTION;
            head->redir->filename = ft_strdup(tokens[*i + 1]);
            *i += 2;
        }
        else
            *i += 1;
    }
}

t_cmds *init_cmds(char **tokens, t_token *type)
{
    t_cmds *head = NULL;
    t_cmds *current = NULL;
    char *full_cmd = NULL;
    char *temp = NULL;
    int i = 0;

    while (tokens[i])
    {
        t_cmds *new_cmd = (t_cmds *)ft_gc_malloc(sizeof(t_cmds));
        if (!new_cmd)
            return (NULL);
        full_cmd = ft_strdup(tokens[i]);
        if (tokens[i + 1] != NULL)
            full_cmd = ft_strjoin(full_cmd, " ");
        while (tokens[i + 1] != NULL && type[i + 1] != PIPE && type[i + 1] != SEMICOLON && type[i + 1] != AND && type[i + 1] != OR && type[i + 1] != HEREDOC && type[i + 1] != INPUT && type[i + 1] != OUTPUT && type[i + 1] != APPEND)
        {
            i++;
            temp = full_cmd;
            full_cmd = ft_strjoin(temp, tokens[i]);
            if (tokens[i + 1] != NULL)
                full_cmd = ft_strjoin(full_cmd, " ");
        }
        new_cmd->cmd = full_cmd;
        new_cmd->redir = NULL;
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

        if (type[i] == PIPE || type[i] == SEMICOLON || type[i] == AND || type[i] == OR)
            i++;
    }
    i = 0;
    set_filename(head, tokens, type, &i);
    return (head);
}


t_line	*init_line(char *line, t_global *global)
{
	t_line	*line_struct;
	t_state	error_state;

	line_struct = ft_gc_malloc(sizeof(t_line));
	if (!line_struct)
		return (NULL);
	error_state = VALID;
	line_struct->token = parsed_line(line);
	line_struct->type = init_tokens_type(line_struct->token);
	error_state = check_errors(line_struct->type, line_struct->token, global);
	if (error_state)
		return (NULL);
	line_struct->cmds = NULL;
	line_struct->pipe = NULL;
	line_struct->heredoc = NULL;
	line_struct->count = count_types(line_struct->type);
	return (line_struct);
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
	manage_exit(&global->exit_code);
	global->env = init_env(envp);
	global->line = NULL;
	return (global);
}
