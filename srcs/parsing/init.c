/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:49:26 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/26 11:25:57 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static t_cmds	*init_cmds(char **tokens, t_token *type)
// {
// 	t_cmds	*head;
// 	t_cmds	*current;
// 	t_cmds	*new_cmd;
// 	char	*full_cmd;
// 	char	*temp;
// 	int		i;
// 	// int		nbr_tokens;

// 	head = NULL;
// 	current = NULL;
// 	i = 0;
// 	new_cmd = (t_cmds *)ft_gc_malloc(sizeof(t_cmds));
// 	if (!new_cmd)
// 		return (NULL);
// 	while (tokens[i] != NULL)
// 	{
// 		full_cmd = ft_strdup(tokens[i]);
// 		if (tokens[i + 1] != NULL)
// 			full_cmd = ft_strjoin(full_cmd, " ");
// 		while (tokens[i + 1] != NULL && type[i + 1] != PIPE && type[i + 1] != SEMICOLON && type[i + 1] != AND && type[i + 1] != OR)
// 		{
// 			i++;
// 			temp = full_cmd;
// 			full_cmd = ft_strjoin(temp, tokens[i]);
// 			if (tokens[i + 1] != NULL)
// 				full_cmd = ft_strjoin(full_cmd, " ");
// 		}
// 		new_cmd->cmd = full_cmd;
// 		new_cmd->redir = NULL;
// 		new_cmd->next = NULL;
// 		if (head == NULL)
// 		{
// 			head = new_cmd;
// 			current = head;
// 		}
// 		else
// 		{
// 			current->next = new_cmd;
// 			current = new_cmd;
// 		}
// 		i++;
// 		// nbr_tokens = i;
// 		// while (tokens[i] != NULL && type[i] != PIPE  && type[i] != SEMICOLON && type[i] != AND && type[i] != OR)
// 		// // while (i < nbr_tokens && tokens[i] != NULL && (type[i] == INPUT || type[i] == OUTPUT || type[i] == APPEND))
// 		// {
// 		// 	if (type[i] == INPUT)
// 		// 	{
// 		// 		new_cmd->input = (t_redirection *)ft_gc_malloc(sizeof(t_redirection));
// 		// 		if (!new_cmd->input)
// 		// 			return (NULL);
// 		// 		new_cmd->input->type = INPUT_REDIRECTION;
// 		// 		new_cmd->input->filename = ft_strdup(tokens[i + 1]);
// 		// 		i += 2;
// 		// 	}
// 		// 	else if (type[i] == OUTPUT)
// 		// 	{
// 		// 		new_cmd->output = (t_redirection *)ft_gc_malloc(sizeof(t_redirection));
// 		// 		if (!new_cmd->output)
// 		// 			return (NULL);
// 		// 		new_cmd->output->type = OUTPUT_REDIRECTION;
// 		// 		new_cmd->output->filename = ft_strdup(tokens[i + 1]);
// 		// 		i += 2;
// 		// 	}
// 		// 	else if (type[i] == APPEND)
// 		// 	{
// 		// 		new_cmd->output = (t_redirection *)ft_gc_malloc(sizeof(t_redirection));
// 		// 		if (!new_cmd->output)
// 		// 			return (NULL);
// 		// 		new_cmd->output->type = APPEND_REDIRECTION;
// 		// 		new_cmd->output->filename = ft_strdup(tokens[i + 1]);
// 		// 		i += 2;
// 		// 	}
// 		// 	else
// 		// 		i++;
// 		// }
// 		if (type[i] == PIPE || type[i] == SEMICOLON || type[i] == AND || type[i] == OR)
// 			i++;
// 	}
// 	i = 0;
// 	while (tokens[i] != NULL && type[i] != PIPE  && type[i] != SEMICOLON && type[i] != AND && type[i] != OR)
// 	// while (i < nbr_tokens && tokens[i] != NULL && (type[i] == INPUT || type[i] == OUTPUT || type[i] == APPEND))
// 	{
// 		if (type[i] == INPUT)
// 		{
// 			new_cmd->redir = (t_redirection *)ft_gc_malloc(sizeof(t_redirection));
// 			if (!new_cmd->redir)
// 				return (NULL);
// 			new_cmd->redir->type = INPUT_REDIRECTION;
// 			new_cmd->redir->filename = ft_strdup(tokens[i + 1]);
// 			i += 2;
// 		}
// 		else if (type[i] == OUTPUT)
// 		{
// 			new_cmd->redir = (t_redirection *)ft_gc_malloc(sizeof(t_redirection));
// 			if (!new_cmd->redir)
// 				return (NULL);
// 			new_cmd->redir->type = OUTPUT_REDIRECTION;
// 			new_cmd->redir->filename = ft_strdup(tokens[i + 1]);
// 			i += 2;
// 		}
// 		else if (type[i] == APPEND)
// 		{
// 			new_cmd->redir = (t_redirection *)ft_gc_malloc(sizeof(t_redirection));
// 			if (!new_cmd->redir)
// 				return (NULL);
// 			new_cmd->redir->type = APPEND_REDIRECTION;
// 			new_cmd->redir->filename = ft_strdup(tokens[i + 1]);
// 			i += 2;
// 		}
// 		else
// 			i++;
// 	}
// 	return (head);
// }

void    set_filename(t_cmds *head, char **tokens, t_token *type, int *i)
{
    while (tokens[*i] != NULL && type[*i] != PIPE  && type[*i] != SEMICOLON && type[*i] != AND && type[*i] != OR)
    {
        if (type[*i] == INPUT)
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
        else if (type[*i] == HEREDOC)
        {
            head->redir = (t_redirection *)ft_gc_malloc(sizeof(t_redirection));
            if (!head->redir)
                return ;
            head->redir->type = HEREDOC_REDIRECTION;
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

    while (tokens[i] != NULL)
    {
        t_cmds *new_cmd = (t_cmds *)ft_gc_malloc(sizeof(t_cmds));
        if (!new_cmd)
            return (NULL);
        full_cmd = ft_strdup(tokens[i]);
        if (tokens[i + 1] != NULL)
            full_cmd = ft_strjoin(full_cmd, " ");
        while (tokens[i + 1] != NULL && type[i + 1] != PIPE && type[i + 1] != SEMICOLON && type[i + 1] != AND && type[i + 1] != OR)
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
	// int i = 0;
	// while (line_struct->token[i])
	// {
	// 	char c = *line_struct->token[i];
	// 	if (*line_struct->token[i] == '\"' || *line_struct->token[i] == '\'')
	// 		line_struct->token[i] = ft_remove_char(line_struct->token[i], c);
	// 	i++;
	// }
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
