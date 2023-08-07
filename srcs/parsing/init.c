/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 13:49:26 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/07 17:06:12 by rrouille         ###   ########.fr       */
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
	t_env	*env;

	env = ft_gc_malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	while (*envp)
	{
		if (!ft_strncmp(*envp, "PATH=", 5))
			env->path = ft_split(*envp + 5, ':');
		else if (!ft_strncmp(*envp, "HOME=", 5))
			env->home = ft_strdup(*envp + 5);
		else if (!ft_strncmp(*envp, "PWD=", 4))
			env->pwd = ft_strdup(*envp + 4);
		else if (!ft_strncmp(*envp, "USER=", 5))
			env->user = ft_strdup(*envp + 5);
		else if (!ft_strncmp(*envp, "SHELL=", 6))
			env->shell = ft_strdup(*envp + 6);
		else if (!ft_strncmp(*envp, "OLDPWD=", 7))
			env->oldpwd = ft_strdup(*envp + 7);
		envp++;
	}
	return (env);
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
	global->cmd = NULL;
	return (global);
}
