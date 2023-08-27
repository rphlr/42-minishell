/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:57:29 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/28 01:22:27 by mariavillar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	char	*tmp;

	if (command[0] == '.' || command[0] == '/')
	{
		if (access(command, F_OK) == 0)
			return (command);
		return (NULL);
	}
	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		path = ft_strjoin(tmp, command);
		if (access(path, F_OK) == 0)
			return (path);
		paths++;
	}
	return (NULL);
}

static int	cmd_is_primaries(char *cmd)
{
	char	*cmd_copy;
	char	*first_word;

	cmd_copy = ft_strdup(cmd);
	first_word = ft_strtok(cmd_copy, " ");
	if (!ft_strcmp(first_word, "echo"))
		return (1);
	else if (!ft_strcmp(first_word, "cd"))
		return (1);
	else if (!ft_strcmp(first_word, "pwd"))
		return (1);
	else if (!ft_strcmp(first_word, "export"))
		return (1);
	else if (!ft_strcmp(first_word, "unset"))
		return (1);
	else if (!ft_strcmp(first_word, "env"))
		return (1);
	else if (!ft_strcmp(first_word, "exit"))
		return (1);
	return (0);
}

static void	execute_primaries(char	*cmd, t_global *global)
{
	char	*cmd_copy;
	char	*first_word;

	cmd_copy = ft_strdup(cmd);
	first_word = ft_strtok(cmd_copy, " ");
	if (!ft_strcmp(first_word, "echo"))
		ft_echo(cmd, global);
	else if (!ft_strcmp(first_word, "cd"))
		ft_cd(cmd, global);
	else if (!ft_strcmp(first_word, "pwd"))
		ft_pwd(global->line);
	else if (!ft_strcmp(first_word, "export"))
		ft_export(global, global->line);
	else if (!ft_strcmp(first_word, "unset"))
		ft_unset(global, global->line);
	else if (!ft_strcmp(first_word, "env"))
		ft_env(global);
	else if (!ft_strcmp(first_word, "exit"))
		ft_exit(global);
}

void	ft_heredoc(char *filename, char *limiter, int type)
{
	int		file;
	int		fd_final;
	char	*buf;

	fd_final = 0;
	file = open(".heredoc_content", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (file < 0)
	{
		ft_printf("Error: failed to open temporary file\n");
		exit (1);
	}
	while (true)
	{
		ft_printf("heredoc> ");
		buf = get_next_line(0);
		if (!buf)
		{
			close(file);
			exit(1);
		}
		if (!ft_strncmp(limiter, buf, ft_strlen(limiter)))
			break ;
		write(file, buf, ft_strlen(buf));
	}
	close(file);
	if (filename)
	{
		if (type == INPUT)
			fd_final = open(filename, O_RDONLY);
		else if (type == APPEND)
			fd_final = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		else if (type == OUTPUT)
			fd_final = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd_final < 0)
		{
			ft_printf("minishell: %s: No such file or directory\n", filename);
			exit (1);
		}
		dup2(fd_final, STDOUT_FILENO);
		close(fd_final);
	}
	fd_final = open(".heredoc_content", O_RDONLY);
	if (fd_final < 0)
	{
		unlink(".heredoc_content");
		ft_printf("Error: failed to open temporary file\n");
		exit (1);
	}
	while ((buf == get_next_line(fd_final)))
		write(STDOUT_FILENO, buf, ft_strlen(buf));
	close(fd_final);
	unlink(".heredoc_content");
}
