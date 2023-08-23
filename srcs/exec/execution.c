/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:57:29 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/23 17:50:14 by rrouille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// faires les pipes

#include "minishell.h"


static char	**env_to_char(t_global *global)
{
	char	**paths;

	if (!global->env)
		return (NULL);
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

	if (command[0] == '.' || command[0] == '/')
	{
		if (access(command, F_OK) == 0)
			return (command);
		return (NULL);
	}
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

void	ft_input(t_cmds *curr_cmd)
{
	int	fd;

	fd = open(curr_cmd->input->filename, O_RDONLY);
	if (fd == -1)
	{
		ft_printf("minishell: %s: %s\n", curr_cmd->input->filename, strerror(errno));
		exit(1);
	}
	printf("fd: %d\n", fd);
	if (dup2(fd, 0) == -1)
	{
		ft_printf("minishell: %s\n", strerror(errno));
		exit(1);
	}
	close(fd);
}


static void	execute_specials(t_global *global, char **paths)
{
	t_count	*count_tmp;
	t_cmds 	*curr_cmd;
	t_token	*type_tmp;

	(void) paths;

	count_tmp = global->line->count;
	curr_cmd = global->line->cmds;
	type_tmp = global->line->type;
	while (count_tmp->special_cases)
	{
		while (!(*type_tmp == INPUT || *type_tmp == OUTPUT || *type_tmp == APPEND || *type_tmp == HEREDOC || *type_tmp == OR || *type_tmp == AND || *type_tmp == SEMICOLON || *type_tmp == PIPE))
			type_tmp++;
		if (count_tmp->nbr_inputs > 0 && *type_tmp == INPUT)
		{
			printf("input\n");
			count_tmp->nbr_inputs--;
			printf ("curr_cmd: %s\n", curr_cmd->cmd);
			ft_input(curr_cmd);
		}
		if (count_tmp->nbr_outputs > 0 && *type_tmp == OUTPUT)
		{
			printf("output\n");
			count_tmp->nbr_outputs--;
			printf ("curr_cmd: %s\n", curr_cmd->cmd);
			// ft_output(global, paths);
		}
		if (count_tmp->nbr_appends > 0 && *type_tmp == APPEND)
		{
			printf("append\n");
			count_tmp->nbr_appends--;
			printf ("curr_cmd: %s\n", curr_cmd->cmd);
			// ft_append(global, paths);
		}
		if (count_tmp->nbr_heredocs > 0 && *type_tmp == HEREDOC)
		{
			printf("heredoc\n");
			count_tmp->nbr_heredocs--;
			printf ("curr_cmd: %s\n", curr_cmd->cmd);
			// ft_heredoc(global, paths);
		}
		if (count_tmp->nbr_ors > 0 && *type_tmp == OR)
		{
			printf("or\n");
			count_tmp->nbr_ors--;
			printf ("curr_cmd: %s\n", curr_cmd->cmd);
			// ft_or(global, paths);
		}
		if (count_tmp->nbr_ands > 0 && *type_tmp == AND)
		{
			printf("and\n");
			count_tmp->nbr_ands--;
			printf ("curr_cmd: %s\n", curr_cmd->cmd);
			// ft_and(global, paths);
		}
		if (count_tmp->nbr_semicolons > 0 && *type_tmp == SEMICOLON)
		{
			printf("semicolon\n");
			count_tmp->nbr_semicolons--;
			printf ("curr_cmd: %s\n", curr_cmd->cmd);
			// ft_semicolon(global, paths);
		}
		if (count_tmp->nbr_pipes > 0 && *type_tmp == PIPE)
		{
			printf("pipe\n");
			count_tmp->nbr_pipes--;
			printf ("curr_cmd: %s\n", curr_cmd->cmd);
			// ft_pipe(global, paths);
		}
		if (!count_tmp->nbr_inputs && !count_tmp->nbr_outputs
			&& !count_tmp->nbr_appends && !count_tmp->nbr_heredocs
			&& !count_tmp->nbr_ors && !count_tmp->nbr_ands
			&& !count_tmp->nbr_semicolons && !count_tmp->nbr_pipes)
			count_tmp->special_cases = false;
		curr_cmd = curr_cmd->next;
		type_tmp++;
	}
}

static void	pid_working(char *path, char **paths, t_global *global)
{
	pid_t	pid;

	pid = fork();
	if (!pid)
	{
		if (execve(path, global->line->token, paths) == -1)
		{
			ft_printf("minishell: %s: %s\n", *global->line->token,
				strerror(errno));
			global->exit_code = 126;
		}
	}
	else if (pid == -1)
	{
		ft_printf("minishell: %s: %s\n", *global->line->token, strerror(errno));
		global->exit_code = 126;
	}
	else
	{
		waitpid(pid, &global->exit_code, 0);
		global->exit_code = WEXITSTATUS(global->exit_code);
	}
}

static void	execute(t_global *global)
{
	char	*path;
	char	**paths;

	paths = env_to_char(global);
	if (!paths)
	{
		ft_printf("minishell: %s: error while loading the path variable\n", *global->line->token);
		global->exit_code = 127;
		return ;
	}
	path = get_path(*global->line->token, paths);
	if (!path)
	{
		ft_printf("minishell: %s: command not found\n", *global->line->token);
		global->exit_code = 127;
		return ;
	}
	if (global->line->count->special_cases == true)
	{
		execute_specials(global, paths);
		return ;
	}
	pid_working(path, paths, global);
}

void	run_cmd(t_global *global)
{
	if (!ft_strcmp(global->line->token[0], "echo"))
		ft_echo(global);
	else if (!ft_strcmp(global->line->token[0], "cd"))
		ft_cd(global);
	else if (!ft_strcmp(global->line->token[0], "pwd"))
		ft_pwd(global->line);
	else if (!ft_strcmp(global->line->token[0], "export"))
		ft_export(global, global->line);
	else if (!ft_strcmp(global->line->token[0], "unset"))
		ft_unset(global, global->line);
	else if (!ft_strcmp(global->line->token[0], "env"))
		ft_env(global);
	else if (!ft_strcmp(global->line->token[0], "exit"))
		ft_exit(global);
	else
	{
		execute(global);
		return ;
	}
	global->exit_code = 0;
}
