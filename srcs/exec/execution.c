/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrouille <rrouille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:57:29 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/15 17:12:59 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**env_to_char(t_global *global)
{
	char	**paths;

	while (ft_strcmp(global->env->name, "PATH"))
	{
		global->env = global->env->next;
		if (!global->env)
			return (NULL);
	}
	paths = ft_gc_malloc(sizeof(char *) * (ft_strlen(global->env->value) + 1));
	if (!paths)
		return (NULL);
	paths = ft_split(global->env->value, ':');
	return (paths);
}

static char	*get_path(char *command, char **paths)
{
	char	*path;
	while (*paths)
	{
		path = ft_strjoin(*paths, "/");
		path = ft_strjoin(path, command);
		if (access(path, F_OK) == 0)
			return (path);
		paths++;
	}
	return (NULL);
}

static void	execute_pipe(t_global *global, char **paths)
{
	(void) paths;
	(void) global;
	ft_printf("execute_pipe\n");
}

void	execute(t_global *global)
{
	char	*path;
	char	**paths;
	int		i;
	pid_t	pid;

	i = 0;
	paths = env_to_char(global);
	if (!paths)
	{
		ft_printf("minishell: %s: command not found\n", *global->cmd->token);
		global->exit_code = 127;
		return ;
	}
	path = get_path(*global->cmd->token, paths);
	if (!path)
	{
		ft_printf("minishell: %s: command not found\n", *global->cmd->token);
		global->exit_code = 127;
	}
	if (global->cmd->nbr_cmd > 1)
	{
		execute_pipe(global, paths);
		return ;
	}
	pid = fork();
	if (!pid)
	{
		if (execve(path, global->cmd->token, paths) == -1)
		{
			ft_printf("minishell: %s: %s\n", *global->cmd->token, strerror(errno));
			global->exit_code = 126;
		}
	}
	else if (pid == -1)
	{
		ft_printf("minishell: %s: %s\n", *global->cmd->token, strerror(errno));
		global->exit_code = 126;
	}
	else
		waitpid(pid, &global->exit_code, 0);
}
