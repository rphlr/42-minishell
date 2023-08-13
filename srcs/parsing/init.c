/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:49:26 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/13 11:42:34 by rrouille         ###   ########.fr       */
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

t_token	*close_quote(char **tokens, t_token *type)
{
	int i = 0;
	int j;
	bool in_single_quote = false;
	bool in_double_quote = false;

	while (tokens[i])
	{
		if (type[i] == NOT_CLOSED_QUOTE)
		{
			in_single_quote = !in_single_quote;
			if (!in_single_quote)
			{
				j = i - 1;
				while (type[j] == NOT_CLOSED_QUOTE)
					j--;
				type[j] = CLOSED_QUOTE;
				type[i] = WORD;
				for (int k = j + 1; k <= i; k++)
				{
					strcat(tokens[j], tokens[k]);
					tokens[k] = NULL;
					type[k] = WORD;
				}
			}
		}
		else if (type[i] == NOT_CLOSED_DQUOTE)
		{
			in_double_quote = !in_double_quote;
			if (!in_double_quote)
			{
				j = i - 1;
				while (type[j] == NOT_CLOSED_DQUOTE)
					j--;
				type[j] = CLOSED_DQUOTE;
				type[i] = WORD;
				for (int k = j + 1; k <= i; k++)
				{
					strcat(tokens[j], tokens[k]);
					tokens[k] = NULL;
					type[k] = WORD;
				}
			}
		}
		i++;
	}
	return type;
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
	t_env	*head = NULL;
	t_env	*current = NULL;

	while (*envp)
	{
		t_env *new_item = ft_gc_malloc(sizeof(t_env));
		if (!new_item)
			return (NULL);

		if (!ft_strncmp(*envp, "PATH=", 5))
		{
			new_item->name = ft_strdup("PATH");
			new_item->value = ft_strdup(*envp + 5);
			new_item->is_env = true;
			// new_item->path = ft_split(*envp + 5, ':');
		}
		else if (!ft_strncmp(*envp, "HOME=", 5))
		{
			new_item->name = ft_strdup("HOME");
			new_item->value = ft_strdup(*envp + 5);
			new_item->is_env = true;
		}
		// ... [Répétez pour les autres variables d'environnement]
		else
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
