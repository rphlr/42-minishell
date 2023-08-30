/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 16:57:29 by rrouille          #+#    #+#             */
/*   Updated: 2023/08/30 14:08:31 by mariavillar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_to_char(t_global *global)
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

char	*get_path(char *command, char **paths)
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

void	create_heredoc_file(void)
{
	int	file;

	file = open(".heredoc_content", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (file < 0)
	{
		ft_printf("Error: failed to open temporary file\n");
		exit(1);
	}
	close(file);
}

void	process_heredoc(char *limiter)
{
	char	*buf;
	int		file;

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
		file = open(".heredoc_content", O_WRONLY | O_APPEND);
		if (file < 0)
		{
			ft_printf("Error: failed to open temporary file\n");
			exit(1);
		}
		write(file, buf, ft_strlen(buf));
		close(file);
	}
}

int	open_and_check(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags, 0644);
	if (fd < 0)
	{
		ft_printf("minishell: %s: No such file or directory\n", filename);
		exit(1);
	}
	return (fd);
}
